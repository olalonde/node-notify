var notify = require("../build/default/gtknotify.node"); // path to our extension

var notification = new notify.notification();
notification.title = "Notification title";
notification.icon = "emblem-default"; // see /usr/share/icons/gnome/16x16
notification.send("Notification message");
notification.icon = "terminal"
notification.send("Second notification... Should be displayed after first one fades out.");
