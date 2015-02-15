{
  "targets": [
    {
      "target_name": "gtknotify",
      "sources": [ "src/node_gtknotify.cpp" ],
      "include_dirs": [
        "<!@(pkg-config libnotify --cflags-only-I | sed s/-I//g)"
      ],
      "libraries": [
        "<!@(pkg-config libnotify --libs)"
      ]
    }
  ]
}
