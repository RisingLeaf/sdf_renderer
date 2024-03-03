#ifndef PTREE_EVENTHANDLER_H
#define PTREE_EVENTHANDLER_H

#include <vector>



namespace EventHandler {
  enum EventType {
    MOUSE_MOVE,
    MOUSE_BUTTONDOWN,
    MOUSE_BUTTONUP,
    KEYDOWN,
    KEYUP,

    NONE,
  };

  struct Event {
    EventType type{EventType::NONE};
    
    // Mouse Move.
    double x{0.};
    double y{0.};

    // Mouse Button.
    int button;

    // Keyboard Key and Mods.
    int key;
    int mods;
  };


  void AddEvent(Event event);
  std::vector<Event> &RetrieveEvents();
};


#endif //PTREE_EVENTHANDLER_H