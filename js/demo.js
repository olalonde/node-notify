var notify = require("../build/default/gtknotify.node"); // path to our extension

var notification = new notify.notification();
notification.title = "Notification title";
notification.icon = "emblem-default"; // see /usr/share/icons/gnome/16x16
notification.send("Notification message");

notification.send("Notification message2");

