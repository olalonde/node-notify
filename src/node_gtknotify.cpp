#include <v8.h>
#include <node.h>
// We need those two libraries for the GTK+ notification 
#include <gtkmm.h>
#include <libnotifymm.h>

// V8 documentation:
// Context: a context is an execution environment that allows separate, unrelated, JavaScript applications to run in a single instance of V8.
//          must be explicitly specified
// Handles: Local (deleted when scope deleted), 
//          Persistent (deleted manually), 
//          Handle (parent class)
// Handle scope: a container that holds lots of handles.  
//               when handle scope's destructor is called (implicitly called when ~ called) all handles created within that scope are removed
// Template: A template is a blueprint for JavaScript functions and objects in a context (i.e. Google Chrome's DOM)
// Function template: A function template is the blueprint for a single function. 
// Object templates: Each function template has an associated object template. accessors/interceptor C++ callbacks

// todo: remove "using namespace" for less confusion node vs v8
using namespace node;
using namespace v8;

class Gtknotify : node::ObjectWrap {
  private:
  public:
    // Function template for notification()
    static v8::Persistent<FunctionTemplate> persistent_function_template;
    
    // Node: the target is the "module", it's where your extension will be loaded into
    static void Init(Handle<Object> target) {
      /* V8 PART */
      // This scope will contain all local handles within this method (in other words, it will contain local_function_template)
      v8::HandleScope scope;

      // Map our function template to the C++ New() method
      // Need to create local template first because that's what v8::FunctionTemplate::New returns
      
      // irc convo
      // <oli> anyone knows why you need to make a local function template before making it persistent?
      // Local<FunctionTemplate> t = FunctionTemplate::New(New); s_ct = Persistent<FunctionTemplate>::New(t);
      // <bradleymeck> oli mainly just readability, you *could* cast it as you assign to t
      // <oli> I assume v8::FunctionTemplate::New returns a local handle
      // <bradleymeck> it does, when im talking about casting here im talking about the v8 api cast not c cast
      v8::Local<FunctionTemplate> local_function_template = v8::FunctionTemplate::New(New);
      
      // Make our local function template persistent
      persistent_function_template = v8::Persistent<FunctionTemplate>::New(local_function_template);
      /* V8 doc:
         Each JavaScript point object keeps a reference to the C++ object for which it is a wrapper with an internal field.
         These fields are so named because they cannot be accessed from within JavaScript, they can only be accessed from C++ code. 
         An object can have any number of internal fields, the number of internal fields is set on the object template as follows: */
      persistent_function_template->InstanceTemplate()->SetInternalFieldCount(1);
      // Map our function template to a Javascript function name, so it can be called from Javascript: Notification()
      persistent_function_template->SetClassName(v8::String::NewSymbol("Notification"));

      /* NODE PART */
      // Arguments: function template, JS method name, C++ method
      // @see https://github.com/joyent/node/blob/v0.2.0/src/node.h#L34
      NODE_SET_PROTOTYPE_METHOD(persistent_function_template, "send", Send);
      // Add .notification() to the target, "module" so we expose our function as module.notification();
      target->Set(String::NewSymbol("Notification"), persistent_function_template->GetFunction());
    }

    Gtknotify() {}

    ~Gtknotify() {}
    
    // Called when our class is created (constructor function)
    // Instantiates C++ object
    static Handle<Value> New(const Arguments& args) {
      HandleScope scope;
      Gtknotify* gtknotify_instance = new Gtknotify();
      // stores a reference to the C++ Gtknotify object inside the args.This() value
      gtknotify_instance->Wrap(args.This()); // Inherited from node::ObjectWrap 
      
      // Once it is wrapped up, it returns args.This(), which is just like what the new operator does in Javascript, 
      // returning the this as an object.
      return args.This();
    }
    
    static v8::Handle<Value> Send(const Arguments& args) {
      v8::HandleScope scope;
      // Extract C++ object reference from JS object passed as args.This()
      Gtknotify* gtknotify_instance = node::ObjectWrap::Unwrap<Gtknotify>(args.This());
      // todo: notify_instance->send();
      //Local<String> result = String::New("Hello World");
      
      // LIBNOTIFY
      v8::String::Utf8Value str(args[0]); // Convert first argument to V8 String
      
      Notify::init("Basic");
      // Arguments: title, content, icon
      Notify::Notification n("Alert", *str, "terminal"); // *str = c string
      // Display notification
      n.show();
      
      return v8::Boolean::New(true);
    }
};

// Why this?
v8::Persistent<FunctionTemplate> Gtknotify::persistent_function_template;
    
/* Thats it for actual interfacing with v8, finally we need to let Node.js know how to dynamically load our code. 
   Because a Node.js extension can be loaded at runtime from a shared object, we need a symbol that the dlsym function can find, 
   so we do the following: */
// @see http://www.freebsd.org/cgi/man.cgi?query=dlsym
// cause of name mangling in C++, we use extern C here
extern "C" {
  static void init(Handle<Object> target) {
    Gtknotify::Init(target);
  }
  // @see http://github.com/ry/node/blob/v0.2.0/src/node.h#L101
  NODE_MODULE(gtknotify, init);
}
