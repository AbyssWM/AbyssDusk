<div align="center">

<img src="https://github.com/AbyssWM.png?size=200" width="160" alt="Abyss logo">

# AbyssDusk

**AbyssDusk – blue light filter for Abyssland, fork of hyprsunset (BSD-3, © hyprwm/vaxerski).**

Part of [**Abyss**](https://github.com/AbyssWM) · Based on [hyprsunset](https://github.com/hyprwm/hyprsunset) by [hyprwm](https://github.com/hyprwm)

</div>

---

> [!WARNING]
> **Early development.** Abyss is being rebranded and reworked right now. Nothing is ready for daily use yet.

## About

AbyssDusk shifts the color temperature and gamma of your displays, based on the time of day or a fixed value. It talks to the compositor through the `abyssland-ctm-control-v1` protocol, so it only works on [Abyssland](https://github.com/AbyssWM/Abyssland).

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

## Credits

AbyssDusk exists thanks to **[hyprsunset](https://github.com/hyprwm/hyprsunset)**, created by **Hypr Development** at **[hyprwm](https://github.com/hyprwm)**. All credit for the original work goes to them – please support the original project.

Abyss is an independent project and is **not affiliated with or endorsed by hyprwm**.

## License

This project is licensed under the **BSD 3-Clause License**. The original copyright notice is kept in [`LICENSE`](LICENSE) as required.

## Contributors

- [bb8TheRobot](https://github.com/bb8TheRobot)
- Claude (Anthropic) – AI pair programmer
