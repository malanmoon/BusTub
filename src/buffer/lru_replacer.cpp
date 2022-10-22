//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_replacer.cpp
//
// Identification: src/buffer/lru_replacer.cpp
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/lru_replacer.h"

namespace bustub {

LRUReplacer::LRUReplacer(size_t num_pages) : page2iter_{num_pages} {}

LRUReplacer::~LRUReplacer() = default;

/**
 * Remove the victim frame as defined by the replacement policy.
 * @param[out] frame_id id of frame that was removed, nullptr if no victim was found
 * @return true if a victim frame was found, false otherwise
 */
bool LRUReplacer::Victim(frame_id_t *frame_id) {
  if(Size() == 0) {
    *frame_id = INVALID_PAGE_ID;
  }
  // singly linked list as LRU cache, each update we move element to the front,
  // element in the back is the least used element (aka victim)
  *frame_id = wait_list_.back(); 
  wait_list_.pop_back(); // remove the victim
  page2iter_[*frame_id] = std::list<frame_id_t>::iterator{}; // empty the page2iter_
  return true;
}

/**
 * Pins a frame, indicating that it should NOT be victimized until it is unpinned.
 * @param frame_id the id of the frame to pin
 */
void LRUReplacer::Pin(frame_id_t frame_id) {
  if (!IsInReplacer(frame_id)) {
    return;
  }
  wait_list_.erase(page2iter_[frame_id]); // remove from wait list
  page2iter_[frame_id] = std::list<frame_id_t>::iterator{}; // empty the page2iter_
}

/**
 * Unpins a frame, indicating that it can now be victimized.
 * @param frame_id the id of the frame to unpin
 */
void LRUReplacer::Unpin(frame_id_t frame_id) {
  if (IsInReplacer(frame_id)) {
    return;
  }
  wait_list_.push_front(frame_id); // put in front of the replacer's list (LRU)
  // for each frame_id, remember it's wait_list_ head
  page2iter_[frame_id] = wait_list_.begin();
}

size_t LRUReplacer::Size() { return wait_list_.size(); }

bool LRUReplacer::IsInReplacer(frame_id_t frame_id) {
  return page2iter_[frame_id] != std::list<frame_id_t>::iterator{};
}

}  // namespace bustub
