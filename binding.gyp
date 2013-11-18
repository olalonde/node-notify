{
  "targets": [
    {
      "target_name": "gtknotify",
      "include_dirs": "/root/src/../api",
      "sources": [ "src/node_gtknotify.cpp" ],
     },
     'link_settings': {
          'libraries': [
              '-lgtkmm-2.4',
              '-llibnotifymm-1.0'
          ]
      }
    ]
}
