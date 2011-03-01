var notify = require("../build/default/gtknotify.node"); // node.js stuff

var notification = new notify.Notification();
//notification.icon = "terminal";
//notification.title = "Notification title";
notification.text = "Some text here...";
console.log(notification.send("yippi"));
