idf_component_register(SRCS 
                        "flappy_bird.c"
                        "state/intro_state.c"
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES
                        led_matrix
                        persistance
)
