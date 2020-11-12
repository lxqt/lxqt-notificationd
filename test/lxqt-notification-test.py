#!/usr/bin/python3
#
# Required: python3, Glib & pydbus
#
import time
from gi.repository import GLib
from pydbus import SessionBus, Variant

# List of example songs for the action test
SONGS = ['Revolution', 'Rocket Man', 'Brainstorm', 'Hard Rock Hallelujah']

class NotificationdTest:
    """
    A small Python class with various tests for lxqt-notificationd
    """
    def __init__(self):
        self.bus = SessionBus()
        self.notifications = self.bus.get(".Notifications")
        self.notifications.onActionInvoked = self.action_handler
        self._playing = False  # keep state for a test
        self._songs_index = 0
        self._id = None

    def _notify(self, summary, body, icon, timeout=-1, actions=None, hints=None, replace=0):
        """
        A simple implementation of a Desktop Notifications Specification 1.2 client.
        For a more detailed explanation see: https://developer.gnome.org/notification-spec/
        """
        return self.notifications.Notify("LXQt-Notificationd Test", replace, icon, summary, body,
                                         list() if actions is None else actions,
                                         dict() if hints is None else hints,
                                         timeout)

    def simple(self):
        """
        Basic notifications
        """
        self._notify("simple notification", "expires in three seconds", "document-open",
                     timeout=3000, hints={'urgency': Variant('b', 0)})  # low urgency
        self._notify("simple notification", "expires in four seconds", "document-close",
                     timeout=4000, hints={'urgency': Variant('b', 1)})  # medium urgency
        self._notify("simple notification", "expires in five seconds", "application-exit",
                     timeout=5000, hints={'urgency': Variant('b', 2)})  # high urgency
        self._notify("simple notification", "expires when the server decides", "go-next", timeout=-1)
        self._notify("simple notification", "never expires.", "go-up", timeout=0)

        long_body = """<i>expires in three seconds</i>. No action there. Lorem ipsum
dolor sit amet, consectetur adipiscing elit. Cras vel leo quam. Morbi
sit amet lorem vel dui commodo porttitor nec ut libero. Maecenas risus
mauris, faucibus id tempus eu, auctor id purus. Vestibulum eget sapien
non sem fermentum fermentum id sed turpis. Morbi pretium sem at turpis
faucibus facilisis vel lacinia ante. Quisque a turpis lectus, quis
posuere magna. Etiam magna velit, sagittis sed tincidunt et,
adipiscing rutrum est. Aliquam aliquam aliquet tortor non
varius. Quisque sollicitudin, ligula ac pulvinar laoreet, lacus metus
sagittis nulla, ac sodales felis diam sed urna. In hac habitasse
platea dictumst. Pellentesque habitant morbi tristique senectus et
netus et malesuada fames ac turpis egestas. Pellentesque habitant
morbi tristique senectus et netus et malesuada fames ac turpis
egestas."""
        self._notify("<b>simple notification</b> with a very long text inside it",
                     long_body, "document-open", timeout=3000, hints={'urgency': Variant('b', 0)})

    def multiple(self):
        """
        Send multiple notifications with the same information which are not update requests
        """
        base = lambda: self._notify("simple notification", "expires in three seconds.", \
                               "document-open", hints={'urgency': Variant('b', 0)})
        base()
        self._notify("simple notifications", "expires in four seconds", "document-close")
        base()
        base()

    def overflow(self):
        """
        Deliberately overflow lxqt-notificationd
        """
        self._notify("tested notification", "is it still the same size?", "", timeout=8000)
        time.sleep(2)

        for i in range(0, 10):
            self._notify("a notification", "four seconds notification {}".format(i), "",
                         timeout=4000)

        for i in range(0, 40):
            self._notify("a notification", "two seconds notification {}".format(i), "",
                         timeout=2000)

    def _send_music_notification(self):
        """
        A simple wrapper which fills in the boilerplate for the music notifications
        """
        state = "start" if self._playing else "pause"
        self._notify("actions notification", SONGS[self._songs_index], 'audio-headphones',
                     actions=['media-skip-backward', 'Previous',
                              'media-playback-{}'.format(state), state,
                              'media-skip-forward', 'Next'],
                     replace=self._id, hints={'action-icons': Variant('b', 1)})

    def actions(self):
        """
        Tests whether lxqt-notificationd properly reacts to actions and whether
        it properly replaces the notifications.

        Since actions are inherently interactive, it makes the layout of the test somewhat
        confusing.
        """
        # Start of by sending a notification without icons
        self._id = self._notify("action notification", "♫ song", "audio-headphones",
                                actions=['prev', 'Previous', 'toggle', 'Play',
                                         'next', 'Next', 'wrong', 'Wrong'],
                                timeout=0)

    def action_handler(self, _, key):
        """
        A handler for the various actions you can invoke
        """
        if key in ('media-skip-backward', 'media-playback-backward', 'prev'):
            self._songs_index += 1
            if self._songs_index == len(SONGS):
                self._songs_index = 0
        elif key in ('media-playback-pause', 'media-playback-start', 'toggle'):
            self._playing = not self._playing
            self._send_music_notification()
        elif key in ('media-skip-forward', 'next'):
            self._songs_index -= 1
            if self._songs_index < 0:
                self._songs_index = len(SONGS) - 1
        elif key == 'wrong':
            # Test the behaviour when the icon cannot be loaded
            self._notify("actions notifications", SONGS[self._songs_index], 'audio-headphones',
                         actions=['media-playback-backward', 'Previous',
                                  'media-playback-start', 'Play',
                                  'media-skip-forward', 'Next'],
                         replace=self._id, hints={'action-icons': Variant('b', 1)})
            return
        else:
            print("Unknown action")
            return

        self._send_music_notification()


if __name__ == "__main__":
    notify = NotificationdTest()
    print(""" What test do you want to run?
1. Basic notifications
2. Multiple notifications
3. Overflow lxqt-notificationd
4. Notification with actions
""")
    choice = int(input("> "))

    if choice == 1:
        notify.simple()
    elif choice == 2:
        notify.multiple()
    elif choice == 3:
        notify.overflow()
    elif choice == 4:
        print("Exit using ^C when you're finished")
        notify.actions()
        GLib.MainLoop().run()
