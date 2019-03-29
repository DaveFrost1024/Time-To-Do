#ifndef TIMETDAPPWINDOW_H
#define TIMETDAPPWINDOW_H

#include <gtkmm.h>

#include <chrono>
#include <thread>
#include <string>

#include <time.h>

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

    void on_hide_window(Gtk::Dialog* dialog = nullptr);

protected:
    Glib::RefPtr<Gtk::Builder> m_refBuilder;
    Glib::RefPtr<Gio::Settings> m_settings;

    Gtk::Button* sButton = nullptr;
    Gtk::Button* pButton = nullptr;
    Gtk::Label* timeLabel = nullptr;

    Gtk::Label* dieLabel = nullptr;
    Gtk::Label* mortalLabel = nullptr;

private:
    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point paused_time;
    long timeElapsed;
    long minuteElapsed;
    long secondElapsed;
    long centisecElapsed;

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
