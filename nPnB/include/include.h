// include.h

#ifndef INCLUDE_H
#define INCLUDE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fstream>
#include <iomanip>
#include <iostream>

#include <chrono>
#include <cstddef>

#include <iterator>
#include <stdexcept>
#include <vector>
#include <set>
#include <time.h>
#include <math.h>
#include <random>
#include <filesystem>
#include <map>
#include <queue>
#include <algorithm>
#include <unistd.h>

#include <unordered_set>
#include <string_view>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <charconv>

class FastProgressPercent {
public:
  explicit FastProgressPercent(std::size_t total)
      : total_(total), start_(clock::now()), last_update_(start_) {}

  void update(const char *name, std::size_t current) {
    if (total_ == 0)
      return;

    std::size_t percent = (current * 100) / total_;

    // 🔥 Only displays if the % changes
    if (percent == last_percent_)
      return;

    auto now = clock::now();
    auto dt = now - last_update_;

    last_update_ = now;
    last_percent_ = percent;

    double seconds = std::chrono::duration<double>(dt).count();
    std::size_t work = current - last_current_;
    last_current_ = current;

    if (work == 0 || seconds <= 0.0)
      return;

    double instant_speed = work / seconds;

    if (!speed_initialized_) {
      speed_ = instant_speed;
      speed_initialized_ = true;
    } else {
      speed_ = alpha_ * instant_speed + (1.0 - alpha_) * speed_;
    }

    if (speed_ <= 0.0)
      return;

    std::size_t remaining = total_ - current;
    std::size_t eta = static_cast<std::size_t>(remaining / speed_);

    std::size_t eta_min = eta / 60;
    std::size_t eta_sec = eta % 60;

    spinner_index_ = (spinner_index_ + 1) & 3;
    static constexpr char spinner[4] = {'|', '/', '-', '\\'};

    std::cerr << '\r' << name << ' ' << spinner[spinner_index_] << ' '
              << percent << "% "
              << "ETA: " << eta_min << "m " << eta_sec << 's' << "   ";
  }

  void done() { std::cerr << "\r"; }

private:
  using clock = std::chrono::steady_clock;

  std::size_t total_;
  clock::time_point start_;
  clock::time_point last_update_;

  std::size_t last_current_ = 0;
  std::size_t last_percent_ = static_cast<std::size_t>(-1);

  double speed_ = 0.0;
  bool speed_initialized_ = false;

  static constexpr double alpha_ = 0.15;

  int spinner_index_ = 0;
};

#endif