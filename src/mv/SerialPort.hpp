#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <array>
#include <boost/asio.hpp>


/**
 * Creates an 8N1 serial port throws boost::system::system_error if
 * there is an error
 *
 * I shouldn't need to link aginst boost because C++11 includes asio (right?)
 * but for some reason I have too.
 */
class SerialPort
{
public:
    SerialPort(boost::asio::io_service& io);
    ~SerialPort();

    void close();
    bool open(const std::string& port, int pin);

    int write(uint8_t *s, const size_t num);
    int write(const std::string& s);
    //int write(const std::vector<uint8_t>& buffer){}

    // size_t read(const size_t number);
    size_t read(const size_t number, uint8_t *buffer);

    void setRTS(bool enabled);
    void setDTR(bool enabled);

protected:
    const bool DD_WRITE, DD_READ;
    void setPin(bool enabled, int pin);
    boost::asio::serial_port serial;
    // std::array<std::uint8_t, 1024> buffer;
    int dir_pin;

#if (__arm__)
    void io_init(int pin){
        dir_pin = pin;

        if (gpioInitialise() < 0) {
            cout << "ERROR: couldn't initialize PiGPIO ... exiting" << endl;
            exit(1);
        }
        else {
            cout << "PiGGPIO is all good!" << endl;
        }

        if (gpioSetMode(pin, PI_OUTPUT) != 0) {
            cout << "ERROR: couldn't set pin direction ... exiting" << endl;
            exit(1);
        }

        if (gpioSetPullUpDown(pin, PI_PUD_UP) != 0) {
            cout << "ERROR: couldn't set up/down resistor ... exiting" << endl;
            exit(1);
        }
    }

    void toggle_direction(bool dir){
        gpioWrite(dir_pin, dir ? 1 : 0);
    }
#else
    void io_init(void){}
    void toggle_direction(bool dir){
        setRTS(dir);
        setDTR(dir);
    }
#endif
};
