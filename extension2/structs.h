typedef enum Colour { RED, AMBER, GREEN };

typedef struct {
    int velocity,
    int acceleration,
    int distance,
} car;

typedef struct {
    Colour colour;
    bool ultrasonic; // true when ultrasonic
} light;

typedef struct {
    int capacity,
    int speed_limit,
} road;

