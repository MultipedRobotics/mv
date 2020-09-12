#include "mv/SerialPort.hpp"
#include <sys/ioctl.h>  // dtr or rts
#include <iostream>

using namespace std;
using namespace boost::asio;
using namespace boost;
// https://www.boost.org/doc/libs/1_65_0/doc/html/boost_asio/reference/serial_port.html
// https://www.boost.org/doc/libs/1_65_0/doc/html/boost_asio/reference/basic_serial_port.html
// http://www.ridgesolutions.ie/index.php/2012/12/13/boost-c-read-from-serial-port-with-timeout-example/

SerialPort::SerialPort(): io(), serial(io), DD_WRITE(false), DD_READ(true), buffer(255){
    // io_init();
}

SerialPort::~SerialPort(){
    close();
}

void SerialPort::close(){
    if (serial.is_open()) serial.close();
}

bool SerialPort::open(const string& port, int pin){return true;}

bool SerialPort::open(const string& port){
    const uint32_t baud_rate = B1000000;
    try {
        serial.open(port);
        cout << ">> port open" << endl;
        // serial.set_option(serial_port_base::baud_rate(baud_rate));
        // cout << ">> baud_rate set" << endl;
        serial.set_option(serial_port::parity(serial_port::parity::none));
        serial.set_option(serial_port::character_size(serial_port::character_size(8)));
        serial.set_option(serial_port::stop_bits(serial_port::stop_bits::one));
        serial.set_option(serial_port::flow_control(serial_port::flow_control::none));

        boost::asio::serial_port_base::baud_rate current_baudrate;
        serial.get_option(current_baudrate);
        cout << termcolor::green << ">> Opened "<< port << " @ "<< current_baudrate.value() << termcolor::reset << endl;

        set_dir(DD_WRITE);
    }
    catch (std::exception const& e){
        cout << termcolor::red << "*** SerialPort::open("<< port << "): " << e.what() << termcolor::reset << endl;
        return false;
    }

    // boost::asio::serial_port_base::flow_control flow_control;
    // serial.get_option(flow_control);
    //
    // boost::asio::serial_port_base::baud_rate gbaud_rate;
    // serial.get_option(gbaud_rate);
    //
    //
    // serial_port_base::stop_bits stop_bits;
    // serial.get_option(stop_bits);
    //
    // serial_port_base::character_size character_size;
    // serial.get_option(character_size);
    //
    // cout << flow_control.value() << " " << character_size.value() << " " << gbaud_rate.value() << " " << stop_bits.value() << endl;
    // exit(0);
    return true;
}

int SerialPort::write(const string& s){
    set_dir(DD_WRITE);
    int ret = asio::write(serial, asio::buffer(s.c_str(),s.size()));
    set_dir(DD_READ);
    return ret;
}

int SerialPort::write(uint8_t *buff, const size_t num){
    set_dir(DD_WRITE);
    int ret = asio::write(serial, asio::buffer(buff,num));
    set_dir(DD_READ);
    return ret;
}

// size_t SerialPort::read(const size_t number){
//     size_t len = asio::read(serial, asio::buffer(&buffer, number));
//     return len;
// }

/**
 packet: [s,s,id,len,err,...]
*/
int SerialPort::read(const size_t number){
    buffer.clear();
    size_t remain = number;
    while(remain > 0){
        remain -= asio::read(serial, asio::buffer(&buffer[number-remain], remain));
    }

    int ret = 0;
    if (buffer[0] == 255 && buffer[1] == 255){
        if (buffer[4] == 0) cout << termcolor::green << "GOOD" << termcolor::reset << endl;
    }

    // TODO: check error code and return it if error
    return ret;
}

// void SerialPort::setPin(bool enabled, int pin){
//     int fd = serial.native_handle();
//     if (enabled)
//         ioctl(fd, TIOCMBIS, &pin);
//     else
//         ioctl(fd, TIOCMBIC, &pin);
// }
//
// void SerialPort::setRTS(bool enabled){
//     // int fd = serial.native_handle();
//     // int data = TIOCM_RTS;
//     // if (!enabled)
//     //     ioctl(fd, TIOCMBIC, &data);
//     // else
//     //     ioctl(fd, TIOCMBIS, &data);
//     setPin(enabled, TIOCM_RTS);
// }
//
// void SerialPort::setDTR(bool enabled){
//     // int fd = serial.native_handle();
//     // int data = TIOCM_DTR;
//     // if (!enabled)
//     //     ioctl(fd, TIOCMBIC, &data);        // Clears the DTR pin
//     // else
//     //     ioctl(fd, TIOCMBIS, &data);        // Sets the DTR pin
//     setPin(enabled, TIOCM_DTR);
// }

void SerialPort::set_dir(bool enabled){
    int fd = serial.native_handle();
    int pin = 0;
    if (enabled){
        pin = TIOCM_RTS;
        ioctl(fd, TIOCMBIS, &pin);
        pin = TIOCM_DTR;
        ioctl(fd, TIOCMBIS, &pin);
    }
    else {
        pin = TIOCM_RTS;
        ioctl(fd, TIOCMBIC, &pin);
        pin = TIOCM_DTR;
        ioctl(fd, TIOCMBIC, &pin);
    }
}
