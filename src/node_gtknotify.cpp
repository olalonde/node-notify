//
// This class is a native C++ Node.js extension for creating GTK+ desktop notification
//
#include <v8.h>
#include <node.h>
// We need those two libraries for the GTK+ notification 
#include <string>
#include <gtkmm.h>
#include <libnotifymm.h>

/* 

= V8 recap =

More detailed info at http://code.google.com/apis/v8/embed.html

== Context ==

A context is an execution environment that allows separate, unrelated, JavaScript applications to run in a single instance of V8. 
Must be explicitly declared.

== Handles ==

Defines the lifetime of an object.

Local (deleted when scope deleted), 
Persistent (deleted manually), 
Handle (parent class)

== Handle scope == 

A container that holds lots of handles.  
When handle scope's destructor is called (implicitly called when ~ called) all handles created within that scope are removed

== Templates ==

A template is a blueprint for JavaScript functions and objects in a context. You can use a template to wrap C++ functions and data structures within JavaScript objects so that they can be manipulated by JavaScript scripts. (i.e. a browser's DOM aka 'document')

Function template: A function template is the blueprint for a single function. 
Object templates: Each function template has an associated object template. accessors/interceptor C++ callbacks

*/

//using namespace node;
using namespace v8;

class Gtknotify : node::ObjectWrap {
  private:
  public:
    Gtknotify() {}
    ~Gtknotify() {}
    
    // Notification title
    std::string title;
    // Notification icon
    // 1) icon file path
    // 2) a 'stock' icon name. see /usr/share/icons
    // i.e. "terminal" or "chrome"
    std::string icon; 
    
    // Holds our constructor function
    static v8::Persistent<FunctionTemplate> persistent_function_template;

    // @Node.js calls Init() when you load the extension through require()
    // Init() defines our constructor function and prototype methods
    // It then binds our constructor function as a property of the target object
    // Target is the "target" onto which an extension is loaded. For example:
    // var notify = require("../build/default/gtknotify.node"); will bind our constructor function to notify.Notification
    // so that we can call "new notify.Notification();"
    static void Init(Handle<Object> target) {
      // We need to declare a V8 scope so that our local handles are eligible for garbage collection.
      // once the Init() returns.
      v8::HandleScope scope;
      // Wrap our New() method so that it's accessible from Javascript
      v8::Local<FunctionTemplate> local_function_template = v8::FunctionTemplate::New(New);
      
      // Make it persistent and assign it to our object's persistent_function_template attribute
      Gtknotify::persistent_function_template = v8::Persistent<FunctionTemplate>::New(local_function_template);
      // Each JavaScript object keeps a reference to the C++ object for which it is a wrapper with an internal field.
      Gtknotify::persistent_function_template->InstanceTemplate()->SetInternalFieldCount(1); // 1 since this is a constructor function
      // Set a class name for objects created with our constructor
      Gtknotify::persistent_function_template->SetClassName(v8::String::NewSymbol("Notification"));
      
      // Set property accessors
      Gtknotify::persistent_function_template->InstanceTemplate()->SetAccessor(String::New("title"), GetTitle, SetTitle);
      Gtknotify::persistent_function_template->InstanceTemplate()->SetAccessor(String::New("icon"), GetIcon, SetIcon);
      
      // @Node.js macro to help bind C++ methods to Javascript methods (see https://github.com/joyent/node/blob/v0.2.0/src/node.h#L34)
      // Arguments: our constructor function, Javascript method na,e, C++ method name
      NODE_SET_PROTOTYPE_METHOD(Gtknotify::persistent_function_template, "send", Send);
      
      // Set the "notification" property to the target and assign it to our constructor function
      target->Set(String::NewSymbol("notification"), Gtknotify::persistent_function_template->GetFunction());
    }

    // new Notification();
    // This is our constructor function. It instantiate a C++ Gtknotify object and returns a Javascript handle to this object.
    static Handle<Value> New(const Arguments& args) {
      HandleScope scope;
      Gtknotify* gtknotify_instance = new Gtknotify();
      // Set some default values
      gtknotify_instance->title = "Node.js";
      gtknotify_instance->icon = "terminal";
      
      // Wrap our C++ object as a Javascript object
      gtknotify_instance->Wrap(args.This());
      
      
      // Our constructor function returns a Javascript object which is a wrapper for our C++ object, 
      // This is the expected behavior when calling a constructor function with the new operator in Javascript.
      return args.This();
    }

    // notification.send();
    // This is a method part of the constructor function's prototype
    static v8::Handle<Value> Send(const Arguments& args) {
      v8::HandleScope scope;
      // Extract C++ object reference from "this" aka args.This() argument
      Gtknotify* gtknotify_instance = node::ObjectWrap::Unwrap<Gtknotify>(args.This());
      
      // Convert first argument to V8 String
      v8::String::Utf8Value v8str(args[0]);
      
      // see http://library.gnome.org/devel/libnotify/0.7/NotifyNotification.html 
      Notify::init("Basic");
      // Args: title, content, icon
      Notify::Notification n(gtknotify_instance->title.c_str(), *v8str, gtknotify_instance->icon.c_str()); // *v8str points to the C string
      // Display notification
      n.show();
      // Return value
      return v8::Boolean::New(true);
    }
    
    // notification.title
    static v8::Handle<Value> GetTitle(v8::Local<v8::String> property, const v8::AccessorInfo& info) {
      // Extract the C++ request object from the JavaScript wrapper.
      Gtknotify* gtknotify_instance = node::ObjectWrap::Unwrap<Gtknotify>(info.Holder());
      return v8::String::New(gtknotify_instance->title.c_str());
    }
    // notification.title=
    static void SetTitle(Local<String> property, Local<Value> value, const AccessorInfo& info) {
      Gtknotify* gtknotify_instance = node::ObjectWrap::Unwrap<Gtknotify>(info.Holder());
      v8::String::Utf8Value v8str(value);
      gtknotify_instance->title = *v8str;
    }
    // notification.icon
    static v8::Handle<Value> GetIcon(v8::Local<v8::String> property, const v8::AccessorInfo& info) {
      // Extract the C++ request object from the JavaScript wrapper.
      Gtknotify* gtknotify_instance = node::ObjectWrap::Unwrap<Gtknotify>(info.Holder());
      return v8::String::New(gtknotify_instance->icon.c_str());
    }
    // notification.icon=
    static void SetIcon(Local<String> property, Local<Value> value, const AccessorInfo& info) {
      Gtknotify* gtknotify_instance = node::ObjectWrap::Unwrap<Gtknotify>(info.Holder());
      v8::String::Utf8Value v8str(value);
      gtknotify_instance->icon = *v8str;
    }
};

// What follows is boilerplate code:

/* Thats it for actual interfacing with v8, finally we need to let Node.js know how to dynamically load our code. 
   Because a Node.js extension can be loaded at runtime from a shared object, we need a symbol that the dlsym function can find, 
   so we do the following: */
// See https://www.cloudkick.com/blog/2010/aug/23/writing-nodejs-native-extensions/ & http://www.freebsd.org/cgi/man.cgi?query=dlsym
// Cause of name mangling in C++, we use extern C here
v8::Persistent<FunctionTemplate> Gtknotify::persistent_function_template;
extern "C" {
  static void init(Handle<Object> target) {
    Gtknotify::Init(target);
  }
  // @see http://github.com/ry/node/blob/v0.2.0/src/node.h#L101
  NODE_MODULE(gtknotify, init);
}
