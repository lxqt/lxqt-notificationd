# lxqt-notificationd

## Overview

`lxqt-notificationd` is LXQt's implementation of a daemon according to the
[Desktop Notifications Specification](https://specifications.freedesktop.org/notification-spec/latest/).

This specification defines a way to display notifications in pop-up windows on
desktops. Notifications like this are e. g. frequently used by chat or mail
clients to inform about incoming messages, by media players to indicate the
beginning of another track but also by the desktop environment itself to indicate
volume changes or similar.

lxqt-notificationd is comprising binaries `lxqt-notificationd` and
`lxqt-config-notificationd`.
`lxqt-notificationd` is running in a daemon-like manner as so-called
[LXQt Module](https://github.com/lxqt/lxqt-session#lxqt-modules) and doing the
actual work. GUI "Desktop Notifications", binary `lxqt-config-notificationd`, is
used to customize the notifications.

lxqt-notificationd is implementing version 1.2 of the said specification.

## Installation

### Compiling source code

Runtime dependencies are KWindowSystem, [liblxqt](https://github.com/lxqt/liblxqt)
and [lxqt-session](https://github.com/lxqt/lxqt-session).
Additional build dependencies are CMake and optionally Git to pull latest VCS
checkouts.

Code configuration is handled by CMake. CMake variable `CMAKE_INSTALL_PREFIX` has
to be set to `/usr` on most operating systems.

To build run `make`, to install `make install` which accepts variable `DESTDIR`
as usual.

### Binary packages

Official binary packages are provided by all major Linux distributions like Arch
Linux, Debian, Fedora and openSUSE. Just use your
package manager to search for string `lxqt-notificationd`.

## Configuration, Usage

As `lxqt-notificationd` is run as LXQt Module it can be adjusted from section
"Basic Settings" in configuration dialogue
[LXQt Session Settings](https://github.com/lxqt/lxqt-session#lxqt-session-settings)
of [lxqt-session](https://github.com/lxqt/lxqt-session).

Configuration dialogue "Desktop Notifications" (binary `lxqt-config-notificationd`)
which is used to customize notifications can be opened from the panel's main
menu - Preferences - LXQt Settings - Desktop Notifications and is provided by the
[Configuration Center](https://github.com/lxqt/lxqt-config#configuration-center)
of [lxqt-config](https://github.com/lxqt/lxqt-config) as well.

### Filtering
It is possible to filter/suppress notifications with a [PCRE](https://www.pcre.org) by setting the
`application_pcre_filter`, `body_pcre_filter`, or `summary_pcre_filter` in the
`lxqt/notifications.conf`. These variables can be set with any valid PCRE with 
each section being checked against its respective PCRE filter. If any of the
PCREs are captured in the application, body, or summary then the message will 
not be shown. The following example `lxqt/notifications.conf` any notification with the
summary containing the words "hello", "goodbye", or "nope" will not be shown:
```text
[General]
...
application_pcre_filter=
body_pcre_filter=
summary_pcre_filter=(?:hello|goodbye|nope)
...
```
https://regex101.com/r/nhtRtx/1

#### Filtering Examples
Filter/block all notifications from the applications: `noisy_app1`,
`noisy_app2`, and `noisy_app3`.
Add the following to lxqt/notification.conf:
```text
[General]
...
applicaiton_pcre_filter=(?:noisy_app1|noisy_app2|noisy_app3)
body_pcre_filter=
summary_pcre_filter=
...
```

Filter/block all notifications containing the strings: `This is a test` and
`This is another test`
```text
[General]
...
applicaiton_pcre_filter=
body_pcre_filter=
summary_pcre_filter=^(?=.*(?:This is a test|This is another test)).*$
...
```
https://regex101.com/r/thW0qo/1


## Translations

Translations can be done in [LXQt-Weblate](https://translate.lxqt-project.org/projects/lxqt-configuration/lxqt-notificationd)

<a href="https://translate.lxqt-project.org/projects/lxqt-configuration/lxqt-notificationd">
<img src="https://translate.lxqt-project.org/widgets/lxqt-configuration/-/lxqt-notificationd/multi-auto.svg" alt="Translation status" />
</a>
