#include "SerialPort.hpp"
#include <sys/ioctl.h>  // dtr or rts
#include <iostream>

using namespace std;
using namespace boost::asio;
using namespace boost;
// https://www.boost.org/doc/libs/1_65_0/doc/html/boost_asio/reference/serial_port.html
// https://www.boost.org/doc/libs/1_65_0/doc/html/boost_asio/reference/basic_serial_port.html
// http://www.ridgesolutions.ie/index.php/2012/12/13/boost-c-read-from-serial-port-with-timeout-example/

SerialPort::SerialPort(boost::asio::io_service& io): serial(io), DD_WRITE(false), DD_READ(true){
    io_init();
}

SerialPort::~SerialPort(){
    close();
}

void SerialPort::close(){
    if (serial.is_open()) serial.close();
}

bool SerialPort::open(const string& port, int pin){
    const uint32_t baud_rate = 1000000;
    try {
        serial.open(port);
        serial.set_option(serial_port_base::baud_rate(baud_rate));
        serial.set_option(serial_port::parity(serial_port::parity::none));
        serial.set_option(serial_port::character_size(serial_port::character_size(8)));
        serial.set_option(serial_port::stop_bits(serial_port::stop_bits::one));
        serial.set_option(serial_port::flow_control(serial_port::flow_control::none));
    }
    catch (std::exception const& e){
        cout << "*** SerialPort::open(): " << e.what() << endl;
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
    return asio::write(serial, asio::buffer(s.c_str(),s.size()));
}

int SerialPort::write(uint8_t *buff, const size_t num){
    return asio::write(serial, asio::buffer(buff,num));
}

// size_t SerialPort::read(const size_t number){
//     size_t len = asio::read(serial, asio::buffer(&buffer, number));
//     return len;
// }

size_t SerialPort::read(const size_t number, uint8_t *buff){
    size_t len = asio::read(serial, asio::buffer(buff, number));
    return len;
}

void SerialPort::setPin(bool enabled, int pin){
    int fd = serial.native_handle();
    if (enabled)
        ioctl(fd, TIOCMBIS, &pin);
    else
        ioctl(fd, TIOCMBIC, &pin);
}

void SerialPort::setRTS(bool enabled){
    // int fd = serial.native_handle();
    // int data = TIOCM_RTS;
    // if (!enabled)
    //     ioctl(fd, TIOCMBIC, &data);
    // else
    //     ioctl(fd, TIOCMBIS, &data);
    setPin(enabled, TIOCM_RTS);
}

void SerialPort::setDTR(bool enabled){
    // int fd = serial.native_handle();
    // int data = TIOCM_DTR;
    // if (!enabled)
    //     ioctl(fd, TIOCMBIC, &data);        // Clears the DTR pin
    // else
    //     ioctl(fd, TIOCMBIS, &data);        // Sets the DTR pin
    setPin(enabled, TIOCM_DTR);
}
