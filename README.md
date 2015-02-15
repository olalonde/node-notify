Tutorial available here: [How to write your own native Node.js extension](http://syskall.com/how-to-write-your-own-native-nodejs-extension) or in TUTORIAL.md

This is a follow up to [How to roll out your own Javascript API with V8](http://syskall.com/how-to-roll-out-your-own-javascript-api-with).


## Install ##

Dependencies(Debian, Ubuntu):

    sudo apt-get install libnotify-dev

Using npm:

    npm install notify
    
Manually:

    node-gyp configure && node-gyp build

## Usage ##

    var notify = require("notify"); // or "../build/Release/gtknotify.node" if you build manually
    var notification = new notify.notification();
    notification.title = "Notification title";
    notification.icon = "emblem-default"; // see /usr/share/icons/gnome/16x16
    notification.send("Notification message"); // using libnotify's default timeout

To send notification with a timeout:

    notification.send("Notification message", 3000); // 3 sec timeout
    // or
    notification.send("Notification message", 0); // lasts forever
    notification.close(); // closes it


## References ##

- [https://www.cloudkick.com/blog/2010/aug/23/writing-nodejs-native-extensions/]()
- [https://github.com/pquerna/node-extension-examples](https://github.com/pquerna/node-extension-examples)
- [http://code.google.com/apis/v8/embed.html](http://code.google.com/apis/v8/embed.html)
- irc.freenode.org #v8 #node.js
- [https://github.com/ncb000gt/node.bcrypt.js](https://github.com/ncb000gt/node.bcrypt.js)
- [https://github.com/joyent/node](https://github.com/joyent/node)
