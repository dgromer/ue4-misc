Activate the plugin in the Plugins dialog (if not already enabled).

Use the Modules dialog (Window -> Developer Tools -> Modules) to recompile and hot-reload the plugin code. Just using the Compile button in the main editor windows does not recompile plugin code.

To write to the parallel port manually: (1) place a MarkerHandler actor into your world, (2) set the Port Address attribute to the address of your parallel port (e.g., 0x3FF8), (3) use the WriteMarkerLPT blueprint function