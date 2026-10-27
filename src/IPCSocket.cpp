#include "IPCSocket.hpp"
#include "AbyssDusk.hpp"
#include "helpers/Log.hpp"

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <format>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <pwd.h>
#include <thread>

void CIPCSocket::initialize() {
    std::thread([&]() {
        const auto SOCKET = socket(AF_UNIX, SOCK_STREAM, 0);

        if (SOCKET < 0) {
            Debug::log(ERR, "Couldn't start the abyssdusk Socket. (1) IPC will not work.");
            return;
        }

        sockaddr_un       SERVERADDRESS = {.sun_family = AF_UNIX};

        const auto        HISenv     = getenv("ABYSSLAND_INSTANCE_SIGNATURE");
        const auto        RUNTIMEdir = getenv("XDG_RUNTIME_DIR");
        const std::string USERID     = std::to_string(getpwuid(getuid())->pw_uid);

        const auto        USERDIR = RUNTIMEdir ? RUNTIMEdir + std::string{"/abyss/"} : "/run/user/" + USERID + "/abyss/";

        std::string       socketPath = HISenv ? USERDIR + std::string(HISenv) + "/.abyssdusk.sock" : USERDIR + ".abyssdusk.sock";

        if (!HISenv)
            mkdir(USERDIR.c_str(), S_IRWXU);

        unlink(socketPath.c_str());

        strcpy(SERVERADDRESS.sun_path, socketPath.c_str());

        bind(SOCKET, (sockaddr*)&SERVERADDRESS, SUN_LEN(&SERVERADDRESS));

        // 10 max queued.
        listen(SOCKET, 10);

        sockaddr_in clientAddress = {};
        socklen_t   clientSize    = sizeof(clientAddress);

        char        readBuffer[1024] = {0};

        Debug::log(LOG, "abyssdusk socket started at {} (fd: {})", socketPath, SOCKET);
        while (1) {
            const auto ACCEPTEDCONNECTION = accept(SOCKET, (sockaddr*)&clientAddress, &clientSize);
            if (ACCEPTEDCONNECTION < 0) {
                Debug::log(ERR, "Couldn't listen on the abyssdusk Socket. (3) IPC will not work.");
                break;
            } else {
                do {
                    Debug::log(LOG, "Accepted incoming socket connection request on fd {}", ACCEPTEDCONNECTION);
                    std::lock_guard<std::mutex> lg(g_pAbyssDusk->m_sEventLoopInternals.loopRequestMutex);

                    auto                        messageSize              = read(ACCEPTEDCONNECTION, readBuffer, 1024);
                    if (messageSize <= 0)
                        break;
                    readBuffer[messageSize == 1024 ? 1023 : messageSize] = '\0';
                    std::string request(readBuffer);

                    m_szRequest     = request;
                    m_bRequestReady = true;

                    g_pAbyssDusk->tick();
                    while (!m_bReplyReady) { // wait for AbyssDusk to finish processing the request
                        std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    }
                    write(ACCEPTEDCONNECTION, m_szReply.c_str(), m_szReply.length());
                    m_bReplyReady = false;
                    m_szReply     = "";

                } while (1);
                Debug::log(LOG, "Closing Accepted Connection");
                close(ACCEPTEDCONNECTION);
            }
        }

        close(SOCKET);
    }).detach();
}

bool CIPCSocket::mainThreadParseRequest() {

    if (!m_bRequestReady)
        return false;

    std::string copy = m_szRequest;

    if (copy == "")
        return false;

    // now we can work on the copy

    Debug::log(LOG, "Received a request: {}", copy);

    // set default reply
    m_szReply       = "ok";
    m_bReplyReady   = true;
    m_bRequestReady = false;

    // config commands
    if (copy.find("gamma") == 0) {
        int spaceSeparator = copy.find_first_of(' ');
        if (spaceSeparator == -1) {
            m_szReply = std::to_string(g_pAbyssDusk->GAMMA * 100);
            return false;
        }

        std::string args     = copy.substr(spaceSeparator + 1);
        float       gamma    = g_pAbyssDusk->GAMMA * 100;
        float       maxGamma = g_pAbyssDusk->MAX_GAMMA * 100;
        try {
            if (args[0] == '+' || args[0] == '-') {
                if (args[0] == '-')
                    gamma -= std::stof(args.substr(1));
                else
                    gamma += std::stof(args.substr(1));
                gamma = std::clamp(gamma, 0.0f, maxGamma);
            } else
                gamma = std::stof(args);
        } catch (std::exception& e) {
            m_szReply = "Invalid gamma value (should be in range 0-" + std::to_string(maxGamma) + "%)";
            return false;
        }

        if (gamma < 0 || gamma > maxGamma) {
            m_szReply = "Invalid gamma value (should be in range 0-" + std::to_string(maxGamma) + "%)";
            return false;
        }

        g_pAbyssDusk->GAMMA = gamma / 100;
        return true;
    }

    if (copy.find("temperature") == 0) {
        int spaceSeparator = copy.find_first_of(' ');
        if (spaceSeparator == -1) {
            m_szReply = std::to_string(g_pAbyssDusk->KELVIN);
            return false;
        }

        std::string        args   = copy.substr(spaceSeparator + 1);
        unsigned long long kelvin = g_pAbyssDusk->KELVIN;
        try {
            if (args[0] == '+' || args[0] == '-') {
                // signed, so going below 0 clamps to 1000 instead of wrapping around to 20000
                const long long DELTA = std::stoll(args.substr(1));
                kelvin                = static_cast<unsigned long long>(std::clamp(static_cast<long long>(kelvin) + (args[0] == '-' ? -DELTA : DELTA), 1000ll, 20000ll));
            } else
                kelvin = std::stoull(args);
        } catch (std::exception& e) {
            m_szReply = "Invalid temperature (should be an integer in range 1000-20000)";
            return false;
        }
        if (kelvin < 1000 || kelvin > 20000) {
            m_szReply = "Invalid temperature (should be an integer in range 1000-20000)";
            return false;
        }

        g_pAbyssDusk->KELVIN   = kelvin;
        g_pAbyssDusk->identity = false;
        return true;
    }

    if (copy.find("identity") == 0) {
        int spaceSeparator = copy.find_first_of(' ');
        if (spaceSeparator == -1) {
            g_pAbyssDusk->identity = true;
            return true;
        }

        std::string args = copy.substr(spaceSeparator + 1);
        if (args == "get") {
            m_szReply = g_pAbyssDusk->identity ? "true" : "false";
            return false;
        } else if (args == "true") {
            g_pAbyssDusk->identity = true;
            return true;
        } else if (args == "false") {
            g_pAbyssDusk->identity = false;
            return true;
        } else {
            m_szReply = "Invalid identity value (should be true or false)";
            return false;
        }
    }

    if (copy.find("reset") == 0) {
        int spaceSeparator = copy.find_first_of(' ');

        // Reset whole profile
        if (spaceSeparator == -1) {
            g_pAbyssDusk->loadCurrentProfile();
            return true;
        }

        if (auto profileOpt = g_pAbyssDusk->getCurrentProfile()) {
            auto        profile = profileOpt.value();
            std::string args    = copy.substr(spaceSeparator + 1);

            if (args == "temperature") {
                g_pAbyssDusk->KELVIN = profile.temperature;
                return true;
            } else if (args == "gamma") {
                g_pAbyssDusk->GAMMA = profile.gamma;
                return true;
            } else if (args == "identity") {
                g_pAbyssDusk->identity = profile.identity;
                return true;
            } else {
                m_szReply = "Invalid reset value (should be either temperature, gamma or identity)";
                return false;
            }
        }
        m_szReply = "No profile is currently loaded";
        return false;
    }

    if (copy.find("profile") == 0) {
        if (auto profileOpt = g_pAbyssDusk->getCurrentProfile()) {
            auto  profile = profileOpt.value();

            int   hrs   = profile.time.hour.count();
            int   mins  = profile.time.minute.count();
            auto  temp  = profile.temperature;
            float gamma = profile.gamma;
            bool  ident = profile.identity;

            m_szReply = std::format("Time: {:0>2}:{:0>2}\nTemperature: {}\nGamma: {}\nIdentity: {}", hrs, mins, temp, gamma, ident);
            return true;
        }
        m_szReply = "No profile is currently loaded";
        return false;
    }

    m_szReply = "invalid command";
    return false;
}
