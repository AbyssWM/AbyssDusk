<div align="center">

<img src="https://github.com/AbyssWM.png?size=200" width="160" alt="Abyss logo">

# AbyssDusk

**Blue light filter for Abyssland.**

Part of [**Abyss**](https://github.com/AbyssWM) · Based on [hyprsunset](https://github.com/hyprwm/hyprsunset) by [hyprwm](https://github.com/hyprwm)

</div>

---

> [!WARNING]
> **Early development.** Abyss is being rebranded and reworked right now. Nothing is ready for daily use yet.

## About

AbyssDusk shifts the color temperature and gamma of your displays, based on the time of day or a fixed value. It talks to the compositor through the `abyssland-ctm-control-v1` protocol, so it only works on [Abyssland](https://github.com/AbyssWM/Abyssland).

AbyssDusk is a fork of [hyprsunset](https://github.com/hyprwm/hyprsunset). Every Abyss library and tool is forked and renamed, so the whole ecosystem builds and updates as one – independent of system packages.

## Install

AbyssDusk builds against the Abyss libraries installed in `/opt/abyss` (Abyssutils, Abyssparse, Abyss-scanner, Abyssland-protocols). From the Abyss workspace:

```sh
sh tools/build.sh AbyssDusk
```

This installs the `abyssdusk` binary and a systemd user unit to `/opt/abyss`.

## Usage

```sh
abyssdusk -t 4000        # fixed temperature in K
abyssdusk -g 90          # gamma in percent
abyssdusk -i             # identity (no color change)
abyssdusk                # use the profiles from the config
```

Config file: `~/.config/abyss/abyssdusk.conf` (Abyssparse format). Example:

```
max-gamma = 100

profile {
    time = 7:30
    identity = true
}

profile {
    time = 20:30
    temperature = 3500
    gamma = 0.8
}
```

The IPC socket lives at `$XDG_RUNTIME_DIR/abyss/$ABYSSLAND_INSTANCE_SIGNATURE/.abyssdusk.sock`.

## The Abyss ecosystem

| Repository | Based on |
|---|---|
| [Abyssland](https://github.com/AbyssWM/Abyssland) | [Hyprland](https://github.com/hyprwm/Hyprland) |
| [Abyssal](https://github.com/AbyssWM/Abyssal) | [aquamarine](https://github.com/hyprwm/aquamarine) |
| [Abyssutils](https://github.com/AbyssWM/Abyssutils) | [hyprutils](https://github.com/hyprwm/hyprutils) |
| [Abyssparse](https://github.com/AbyssWM/Abyssparse) | [hyprlang](https://github.com/hyprwm/hyprlang) |
| [Abysscursor](https://github.com/AbyssWM/Abysscursor) | [hyprcursor](https://github.com/hyprwm/hyprcursor) |
| [Abyssgraphics](https://github.com/AbyssWM/Abyssgraphics) | [hyprgraphics](https://github.com/hyprwm/hyprgraphics) |
| [Abysswire](https://github.com/AbyssWM/Abysswire) | [hyprwire](https://github.com/hyprwm/hyprwire) |
| [Abyss-scanner](https://github.com/AbyssWM/Abyss-scanner) | [hyprwayland-scanner](https://github.com/hyprwm/hyprwayland-scanner) |
| [Abyssland-protocols](https://github.com/AbyssWM/Abyssland-protocols) | [hyprland-protocols](https://github.com/hyprwm/hyprland-protocols) |
| [Xdg-desktop-portal-abyss](https://github.com/AbyssWM/Xdg-desktop-portal-abyss) | [xdg-desktop-portal-hyprland](https://github.com/hyprwm/xdg-desktop-portal-hyprland) |
| [Abysstoolkit](https://github.com/AbyssWM/Abysstoolkit) | [hyprtoolkit](https://github.com/hyprwm/hyprtoolkit) |
| [Abyss-guiutils](https://github.com/AbyssWM/Abyss-guiutils) | [hyprland-guiutils](https://github.com/hyprwm/hyprland-guiutils) |
| **AbyssDusk** | [hyprsunset](https://github.com/hyprwm/hyprsunset) |
| [AbyssLock](https://github.com/AbyssWM/AbyssLock) | [hyprlock](https://github.com/hyprwm/hyprlock) |
| [AbyssPaper](https://github.com/AbyssWM/AbyssPaper) | [hyprpaper](https://github.com/hyprwm/hyprpaper) |
| [AbyssPicker](https://github.com/AbyssWM/AbyssPicker) | [hyprpicker](https://github.com/hyprwm/hyprpicker) |

## Credits

AbyssDusk exists thanks to **[hyprsunset](https://github.com/hyprwm/hyprsunset)**, created by **Hypr Development** at **[hyprwm](https://github.com/hyprwm)**. All credit for the original work goes to them – please support the original project.

Abyss is an independent project and is **not affiliated with or endorsed by hyprwm**.

## License

This project is licensed under the **BSD 3-Clause License**. The original copyright notice is kept in [`LICENSE`](LICENSE) as required.

## Contributors

- [bb8TheRobot](https://github.com/bb8TheRobot)
- Claude (Anthropic) – AI pair programmer
