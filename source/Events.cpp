#include "Events.h"

#include <mutex>



namespace EventHandler {
  std::mutex eventMutex;
  std::vector<Event> eventList;

  void AddEvent(Event event)
  {
    std::lock_guard<std::mutex> guard(eventMutex);
    eventList.emplace_back(event);
  };

  std::vector<Event> &RetrieveEvents()
  {
    std::lock_guard<std::mutex> guard(eventMutex);

    static std::vector<Event> eventListCopy;

    eventListCopy.clear();
    eventList.swap(eventListCopy);

    return eventListCopy;
  }
};
