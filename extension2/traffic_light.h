typedef enum Colour { RED, RED_AMBER, GREEN, AMBER};

typedef struct {
    Colour colour;
    bool ultrasonic; // true when ultrasonic
} light;
