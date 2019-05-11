#include <cmath>
#include <chrono>
#include <cstdio>
#include <thread>
#include <iostream>

extern "C" {
#include "wooting-rgb-sdk.h"
}

static bool contrun;

bool isRunning(const char* name) {
   char command[32];
   sprintf(command, "pgrep -x %s > /dev/null", name);
   return 0 == system(command);
}


void wooting_rgb_clr() {
    for (int i = 0; i <=20; i++) {
        for (int j = 0; j <= 5; j++) {
            wooting_rgb_array_set_single(j, i, 0, 0, 0);
        }
        if (i != 0) {
            for (int j = 0; j <= 5; j++) {
                wooting_rgb_array_set_single(j, i-1, 0, 0, 0);
            }
        }
    }
    wooting_rgb_array_update_keyboard();
    std::cout << "Keyboard RGB Cleared" << std::endl;
}

void wooting_rgb_scan(int negate) {
    if (negate > 0) {
        for (int i = 0; i <= 20; i++) {
            for (int j = 0; j <= 5; j++) {
                wooting_rgb_array_set_single(j, i, 0, 255, 0);
            }
            if (i != 0) {
                for (int j = 0; j <= 5; j++) {
                    wooting_rgb_array_set_single(j, i-1, 0, 0, 0);
                }
            }
            wooting_rgb_array_update_keyboard();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    } else if (negate < 0) {
        for (int i = 20; i >= 0; i--) {
            for (int j = 0; j <= 5; j++) {
                wooting_rgb_array_set_single(j, i, 0, 255, 0);
            }
            if (i != 20) {
                for (int j = 0; j <= 5; j++) {
                    wooting_rgb_array_set_single(j, i+1, 0, 0, 0);
                }
            }
            wooting_rgb_array_update_keyboard();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}

void thrdloop() {
    while (contrun) {
        if (isRunning("swaylock")) {
            wooting_rgb_scan(1);
            wooting_rgb_scan(-1);
        } else {
            wooting_rgb_reset();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

int main() {
    std::cout << "G'day" << std::endl;
    std::cout << "Keyboard Connection Status: " << wooting_rgb_kbd_connected() << std::endl;
    if (!wooting_rgb_kbd_connected()) {
        return 1;
    }
    wooting_rgb_reset();
    contrun = true;
    std::thread scanloop(thrdloop);
    std::string input;
    while (contrun) {
        getline (std::cin, input);
        if (input.compare("exit") == 0) {
            contrun = false;
            scanloop.join();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    wooting_rgb_reset();
    return 0;
}

