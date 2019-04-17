#ifndef TIMETDAPPWINDOW_H
#define TIMETDAPPWINDOW_H

#include <gtkmm.h>

#include <chrono>
#include <thread>
#include <string>

#include <time.h>
#include "TimetdApplication.h"

class TimetdAppWindow : public Gtk::ApplicationWindow
{
public:
    TimetdAppWindow(BaseObjectType* cobject,
        const Glib::RefPtr<Gtk::Builder>& refBuilder);

    static TimetdAppWindow* create();

    void set_mortality();
    bool update_mortality();

    void begin_timer();
    bool update_timer();
    void pause_timer();

    void begin_countdown();
    bool update_countdown();

    void on_hide_window(Gtk::Dialog* dialog = nullptr);

protected:
    Glib::RefPtr<Gtk::Builder> m_refBuilder;
    Glib::RefPtr<Gio::Settings> m_settings;

    Gtk::Button* sButton = nullptr;
    Gtk::Button* pButton = nullptr;
    Gtk::Label* timeLabel = nullptr;

    Gtk::Entry* secondEntry = nullptr;
    Gtk::Entry* minuteEntry = nullptr;
    Gtk::Button* tsButton = nullptr;

    Gtk::Label* dieLabel = nullptr;
    Gtk::Label* mortalLabel = nullptr;

private:
    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point paused_time;
    long timeElapsed;
    long minuteElapsed;
    long secondElapsed;
    long centisecElapsed;

    // countdown variables
    std::chrono::high_resolution_clock::time_point countdown_Start;
    std::chrono::high_resolution_clock::time_point countdown_Pause;
    long secondRemaining;
    bool countdownStarted = false;
    bool countdownPaused = false;

    // mortal variables
    const long yearInSeconds = 31536000;
    const long dayInSeconds = 86400;

    time_t mortalTimeRaw;
    struct tm * deadTime;
    double mortalSeconds;
    int remainYear;
    int remainDay;
    int remainHour;
    int remainMinute;
    int remainSecond;
    int averageLife;
    int birthYear;
    int birthMonth;
    int birthDay;

    bool started = false;
    bool paused = false;

    bool showCentiSec;

};

#endif /* TIMETDAPPWINDOW_H */
