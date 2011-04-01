__THIS IS A WORK IN PROGRESS! THE TUTO WILL BE UP ON [HTTP://SYSKALL.COM](HTTP://SYSKALL.COM) WITHIN A FEW DAYS__

Part 1 (building a V8 extension) available here: [http://syskall.com/how-to-roll-out-your-own-javascript-api-with](http://syskall.com/how-to-roll-out-your-own-javascript-api-with)

## Install ##

Using npm:

    npm install notify
    
Manually:

    node-waf configure && node-waf build

## Usage ##

    var notify = require("notify"); // or "../build/default/gtknotify.node" if you build manually
    var notification = new notify.notification();
    notification.title = "Notification title";
    notification.icon = "emblem-default"; // see /usr/share/icons/gnome/16x16
    notification.send("Notification message");

## References ##

- [https://www.cloudkick.com/blog/2010/aug/23/writing-nodejs-native-extensions/]()
- [https://github.com/pquerna/node-extension-examples](https://github.com/pquerna/node-extension-examples)
- [http://code.google.com/apis/v8/embed.html](http://code.google.com/apis/v8/embed.html)
- irc.freenode.org #v8 #node.js
- [https://github.com/ncb000gt/node.bcrypt.js](https://github.com/ncb000gt/node.bcrypt.js)
- [https://github.com/joyent/node](https://github.com/joyent/node)
