#include "TimetdAppwindow.h"
#include <iostream>
#include <stdexcept>
#include <iomanip>

#include <math.h>

TimetdAppWindow::TimetdAppWindow(BaseObjectType* cobject,
    const Glib::RefPtr<Gtk::Builder>& refBuilder)
    : Gtk::ApplicationWindow(cobject),
    m_refBuilder(refBuilder),
    m_settings()
{

    m_settings = Gio::Settings::create("org.gtkmm.timetodo");

    showCentiSec = m_settings->get_boolean("centisecond");

    //std::cout << m_settings->get_int("average-life-expectancy") << "\n";

    // connect button click signal to corresponding functions
    refBuilder->get_widget("start_button", sButton);
    if (sButton)
    {
      sButton->signal_clicked().connect( sigc::mem_fun(*this, &TimetdAppWindow::begin_timer) );
    }

    refBuilder->get_widget("pause_button", pButton);
    if (pButton)
    {
      pButton->signal_clicked().connect( sigc::mem_fun(*this, &TimetdAppWindow::pause_timer) );
    }

    m_refBuilder->get_widget("time", timeLabel);

    if (!showCentiSec)
    {
      timeLabel->set_text("0:00");
    }


    m_refBuilder->get_widget("die_label", dieLabel);

    m_refBuilder->get_widget("mortal_label", mortalLabel);

    set_mortality();

    // setup timeout to periodically call update_mortality()
    sigc::slot<bool> mortal_slot = sigc::mem_fun(*this, &TimetdAppWindow::update_mortality);
    sigc::connection conn = Glib::signal_timeout().connect(mortal_slot, 1000);
}

//static
TimetdAppWindow* TimetdAppWindow::create()
{
    // Load the Builder file and instantiate its widgets.
    auto refBuilder = Gtk::Builder::create_from_resource("/org/gtkmm/timetodo/window.glade");

    TimetdAppWindow* window = nullptr;
    refBuilder->get_widget_derived("app_window", window);
    if (!window)
    {
        throw std::runtime_error("No \"app_window\" object in window.glade");
    }

    Glib::ustring data = ".btn_front_font{ font-size: 24px;} .mortal_font{ font-size: 32px;}";

    // load from css file
    auto refProvider = Gtk::CssProvider::create();
    auto refStyleContext = Gtk::StyleContext::create();
    //refProvider->load_from_path(/*"/home/david/Desktop/Github-Projects/Time-To-Do/timeToDo.css"*/"../timeToDo.css");
    refProvider->load_from_data(data);

    Glib::RefPtr<Gdk::Screen> refScreen = window->get_screen();
    refStyleContext->add_provider_for_screen(refScreen, refProvider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    return window;
}

void TimetdAppWindow::on_hide_window(Gtk::Dialog* dialog)
{
    // update preference on centi second
    showCentiSec = m_settings->get_boolean("centisecond");

    averageLife = std::stoi(m_settings->get_string("average-life-expectancy"));
    birthYear = std::stoi(m_settings->get_string("birthyear"));

    if (!showCentiSec && !started)
    {
      timeLabel->set_text("0:00");
    }
    else if (!started)
    {
      timeLabel->set_text("0:00:00");
    }

    set_mortality();
}

void TimetdAppWindow::set_mortality()
{
  averageLife = std::stoi(m_settings->get_string("average-life-expectancy"));
  birthYear = std::stoi(m_settings->get_string("birthyear"));
  birthMonth = std::stoi(m_settings->get_string("birthmonth"));
  birthDay = std::stoi(m_settings->get_string("birthday"));

  time( &mortalTimeRaw );
  deadTime = localtime( &mortalTimeRaw );
  deadTime->tm_year = birthYear - 1900 + averageLife;
  deadTime->tm_mon = birthMonth - 1;
  deadTime->tm_mday = birthDay;
  deadTime->tm_hour = 0; deadTime->tm_min = 0; deadTime->tm_sec = 0;

  mortalSeconds = difftime(mktime(deadTime), mortalTimeRaw);

  //std::cout << std::fixed << std::setprecision(0) << mortalSeconds << "\n";

  remainYear = mortalSeconds / (60 * 60 * 24 * 365);
  remainDay = ( mortalSeconds - (60 * 60 * 24 * 365 * remainYear) ) / (60 * 60 * 24);
  remainHour = ( mortalSeconds - ( 60 *60 * 24 * 365 * remainYear + 60 * 60 * 24 * remainDay ) ) / (60 * 60);
  remainMinute = ( mortalSeconds - ( 60 * 60 * 24 * 365 * remainYear + 60 * 60 * 24 * remainDay + 60 * 60 * remainHour ) ) / 60;
  remainSecond = mortalSeconds - ( 60 * 60 * 24 * 365 * remainYear + 60 * 60 * 24 * remainDay + 60 * 60 * remainHour + 60 * remainMinute );

  if (dieLabel)
  {
    dieLabel->set_text("Assuming you live until " + std::to_string(averageLife) + ", you have:");
  }

  if (mortalLabel)
  {
    mortalLabel->set_text(std::to_string(remainYear) + " years " + std::to_string(remainDay) + " days\n" +
      std::to_string(remainHour) + " hours " + std::to_string(remainMinute) + " minutes " +
      std::to_string(remainSecond) + " seconds");
  }
}

bool TimetdAppWindow::update_mortality()
{
  time( &mortalTimeRaw );
  deadTime = localtime( &mortalTimeRaw );
  deadTime->tm_year = birthYear - 1900 + averageLife;
  deadTime->tm_mon = birthMonth - 1;
  deadTime->tm_mday = birthDay;
  deadTime->tm_hour = 0; deadTime->tm_min = 0; deadTime->tm_sec = 0;

  mortalSeconds = difftime(mktime(deadTime), mortalTimeRaw);

  //std::cout << std::fixed << std::setprecision(0) << mortalSeconds << "\n";

  remainYear = mortalSeconds / yearInSeconds;
  remainDay = ( mortalSeconds - (yearInSeconds * remainYear) ) / (dayInSeconds);
  remainHour = ( mortalSeconds - ( yearInSeconds * remainYear + dayInSeconds * remainDay ) ) / (3600);
  remainMinute = ( mortalSeconds - ( yearInSeconds * remainYear + dayInSeconds * remainDay + 3600 * remainHour ) ) / 60;
  remainSecond = mortalSeconds - ( yearInSeconds * remainYear + dayInSeconds * remainDay + 3600 * remainHour + 60 * remainMinute );

  if (mortalLabel)
  {
    mortalLabel->set_text(std::to_string(remainYear) + " years " + std::to_string(remainDay) + " days\n" +
      std::to_string(remainHour) + " hours " + std::to_string(remainMinute) + " minutes " +
      std::to_string(remainSecond) + " seconds");
  }

  return true;
}

void TimetdAppWindow::begin_timer()
{
    if (started)
    {
        started = false;

        if (!showCentiSec)
        {
          timeLabel->set_text("0:00");
        }
        else
        {
          timeLabel->set_text("0:00:00");
        }
        sButton->set_label("Start");
    }
    else
    {
        started = true;

        sButton->set_label("Stop");

        start_time = std::chrono::high_resolution_clock::now();

        // use this if you need to put parameters
        //sigc::slot<bool> my_slot = sigc::bind(sigc::mem_fun(*this, &TimetdAppWindow::begin_timer), 0);

        // setup timeout to periodically call update_timer()
        sigc::slot<bool> my_slot = sigc::mem_fun(*this, &TimetdAppWindow::update_timer);
        sigc::connection conn = Glib::signal_timeout().connect(my_slot, 10);
    }
}

bool TimetdAppWindow::update_timer()
{
    if (paused)
    {
      return true;
    }
    else if (!started)
    {
      return false;
    }

    std::chrono::high_resolution_clock::time_point current_time = std::chrono::high_resolution_clock::now();

    auto time_diff = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
    timeElapsed = time_diff;

    minuteElapsed = timeElapsed / 60000;
    secondElapsed = timeElapsed % 60000 / 1000;

    std::string secondString;

    if (secondElapsed < 10)
    {
        secondString = "0" + std::to_string(secondElapsed);
    }
    else
    {
        secondString = std::to_string(secondElapsed);
    }



    if (!showCentiSec)
    {
        timeLabel->set_text(std::to_string(minuteElapsed) + ":" + secondString);
    }
    else
    {
        centisecElapsed = timeElapsed % 1000 / 10;
        std::string centisecString;

        if (centisecElapsed < 10)
        {
            centisecString = "0" + std::to_string(centisecElapsed);
        }
        else
        {
            centisecString = std::to_string(centisecElapsed);
        }

        timeLabel->set_text(std::to_string(minuteElapsed) + ":" + secondString + ":" + centisecString);
    }

    return true;
}

void TimetdAppWindow::pause_timer()
{
    if (!started)
    {
      return;
    }

    if (paused)
    {
        pButton->set_label("Pause");

        std::chrono::high_resolution_clock::time_point current_time = std::chrono::high_resolution_clock::now();

        auto time_diff = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - paused_time).count();
        start_time += std::chrono::milliseconds(time_diff);

        paused = false;
    }
    else
    {
        pButton->set_label("Resume");

        paused_time = std::chrono::high_resolution_clock::now();

        paused = true;
    }
}
