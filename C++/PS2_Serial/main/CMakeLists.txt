add_executable(main
         main.c
         )

 # Import pico_stdlib which imports commonly used features
 target_link_libraries(main pico_stdlib)

 # Create map/bin/hex file etc.
 pico_add_extra_outputs(main)

 # Add path via pico_set_program_url CMAKE
 example_auto_set_url(main)
 