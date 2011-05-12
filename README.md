Tutorial available here: [How to write your own native Node.js extension](http://syskall.com/how-to-write-your-own-native-nodejs-extension) or in TUTORIAL.md

This is a follow up to [How to roll out your own Javascript API with V8](http://syskall.com/how-to-roll-out-your-own-javascript-api-with).

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
