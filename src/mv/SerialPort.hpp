#pragma once

#include <stdint.h> // int types
#include <string>
#include <vector>
#include <array> // buffer
#include <sys/ioctl.h>
#include <string.h> // memset
// #include <boost/asio.hpp>
// #include <sys/stat.h>
#include <fcntl.h> // open
#include <termcolor/termcolor.hpp>
#include <mv/common.hpp> // structures
#include <unistd.h> // sleep
#include <termios.h> // serial

constexpr bool DD_WRITE = false;  // false
constexpr bool DD_READ = !DD_WRITE;

void msleep(unsigned int msec){
    usleep(1000*msec);
}

class Serial {
    int fd;
    // const bool DD_WRITE, DD_READ;
    // packet buffer;
    std::array<std::uint8_t, 512> buffer;

public:
    Serial(): fd(0) {}
    ~Serial(){close();}

    bool open(const std::string& port){
        struct termios t;
        // int baud = B1000000;

        fd = ::open(port.c_str(), O_RDWR|O_NOCTTY|O_NONBLOCK);
        if(fd < 0){
            perror("Error opening serial port");
            return false;
        }

        memset(&t, 0, sizeof(t)); // clear struct for new port settings

        // t.c_cflag = baud | CS8 | CLOCAL | CREAD;
        t.c_cflag = B1000000 | CS8 | CLOCAL | CREAD;
        // t.c_cflag = 0010010 | CS8 | CLOCAL | CREAD;
        t.c_iflag = IGNPAR;
        t.c_oflag      = 0;
        t.c_lflag      = 0;
        t.c_cc[VTIME]  = 0; // 10th of second
        t.c_cc[VMIN]   = 0;

        // clean the buffer and activate the settings for the port
        tcflush(fd, TCIFLUSH);
        if (tcsetattr(fd, TCSANOW, &t) < 0) perror("*** Couldn't set port attribute");

        if (tcgetattr(fd, &t) < 0) perror("Couldn't get port attribute");
        printf(">> %u %u \n", cfgetispeed(&t), cfgetospeed(&t));

        set_dir(DD_READ);
        msleep(100);
        set_dir(DD_WRITE);

        return true;
    }

    void close(){
        if (fd > 0) ::close(fd);
    }

    int write(const packet& buff){
        set_dir(DD_WRITE);
        int ret = ::write(fd, buff.data(), buff.size());
        // for (const auto& p: buff) ::write(fd, (void*)&p, 1);
        set_dir(DD_READ);
        flush();
        return ret;
        // return 0;
    }

    int read(const size_t number){
        // buffer.clear();
        buffer.fill(0);
        size_t remain = number;
        while(remain > 0){
            remain -= ::read(fd, buffer.data(), remain);
        }

        int ret = 0;
        if (buffer[0] == 255 && buffer[1] == 255){
            // if (buffer[4] == 0) cout << termcolor::green << "GOOD" << termcolor::reset << endl;
        }

        // TODO: check error code and return it if error
        return ret;
    }

    packet read(){
        // buffer.clear();
        buffer.fill(0);
        int num = available();
        if (num == 0) {
            packet pkt;
            return pkt;
        }

        size_t remain = num;
        while(remain > 0){
            remain -= ::read(fd, buffer.data(), remain);
        }

        // int ret = 0;
        // if (buffer[0] == 255 && buffer[1] == 255){
        //     // if (buffer[4] == 0) cout << termcolor::green << "GOOD" << termcolor::reset << endl;
        // }

        // for (int i=0; i<num; ++i) std::cout << int(buffer[i]) << ',';
        // std::cout << std::endl;

        auto pkt = packet(num);
        std::copy(buffer.begin(), buffer.begin() + num, pkt.begin());

        // TODO: check error code and return it if error
        return pkt;
    }

    void flush(){
        tcflush(fd, TCIFLUSH);
    }

    int available(){
        int bytes_available;
        ioctl(fd, FIONREAD, &bytes_available);
        return bytes_available;
    }

    void set_dir(bool enabled){
        // int fd = serial.native_handle();
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
};

//////////////////////////////////////
































/**
 * Creates an 8N1 serial port throws boost::system::system_error if
 * there is an error
 *
 * I shouldn't need to link aginst boost because C++11 includes asio (right?)
 * but for some reason I have too.
 */
// class SerialPort
// {
// public:
//     SerialPort();
//     ~SerialPort();
//
//     void close();
//     bool open(const std::string& port);
//     bool open(const std::string& port, int pin);
//
//     void set_speed(unsigned int speed){
//         termios t;
//         int fd = serial.native_handle();
//
//         // if (tcgetattr(fd, &t) < 0) { std::cout << "get attr error" << std::endl; }
//         // if (cfsetspeed(&t, B1000000) < 0) { std::cout << "config speed error" << std::endl; }
//
//         bzero(&t, sizeof(t));
//         t.c_cflag = CS8 | CLOCAL | CREAD;
//         t.c_iflag = IGNBRK | IGNPAR;
//         t.c_oflag = 0;
//         t.c_lflag = 0;
//         // cfsetispeed(&t, B921600);
//         // cfsetospeed(&t, B921600);
//
//         cfgetispeed(&t);
//         tcflush(fd, TCIFLUSH);
//
//         tcsetattr(fd, TCSANOW, &t);
//
//         // std::cout << termcolor::green << ">> Rate " << current_baudrate.value() << termcolor::reset << std::endl;
//
//         // if (tcsetattr(fd, TCSANOW, &t) < 0) { perror("set speed error"); }
//
//         // Speed changed
//         boost::asio::serial_port_base::baud_rate current_baudrate;
//         serial.get_option(current_baudrate);
//         std::cout << termcolor::blue << ">> Rate " << current_baudrate.value() << termcolor::reset << std::endl;
//
//     }
//
//     int write(uint8_t *s, const size_t num);
//     int write(const std::string& s);
//     //int write(const std::vector<uint8_t>& buffer){}
//
//     // size_t read(const size_t number);
//     int read(const size_t number);
//     // int readByte(){
//     //     // int LOOP_LIMIT = 5;
//     //     // int loop = 0;
//     //     //
//     //     // while (availableData() < 6){
//     //     //     if (loop < LOOP_LIMIT) return 0;
//     //     //     loop++;
//     //     //     // delayus(10);
//     //     // }
//     //
//     //     while (availableData() > 0){
//     //         Incoming_Byte = serial.read();
//     //         if ( (Incoming_Byte == 255) & (peekData() == 255) ){
//     //             serial.read();                               // Start Bytes
//     //             serial.read();                               // Ax-12 ID
//     //             serial.read();                               // Length
//     //             if( (Error_Byte = readData()) != 0 )      // Error
//     //                 return (Error_Byte*(-1));
//     //             buffer[0] = readData();
//     //         }
//     //     }
//     //     return 0;
//     //
//     // }
//
//     // void setRTS(bool enabled);
//     // void setDTR(bool enabled);
//     void set_dir(bool enable);
//
//     int availableData(){return 0;} // FIXME
//     int peekData(){return 0;} // FIXME
//
// protected:
//     const bool DD_WRITE, DD_READ;
//     // void setPin(bool enabled, int pin);
//     boost::asio::io_service io;
//     boost::asio::serial_port serial;
//     packet buffer;
//
//     // std::array<std::uint8_t, 1024> buffer;
//     // int dir_pin;
//
// // #if (__arm__)
// //     void io_init(int pin){
// //         dir_pin = pin;
// //
// //         if (gpioInitialise() < 0) {
// //             cout << "ERROR: couldn't initialize PiGPIO ... exiting" << endl;
// //             exit(1);
// //         }
// //         else {
// //             cout << "PiGGPIO is all good!" << endl;
// //         }
// //
// //         if (gpioSetMode(pin, PI_OUTPUT) != 0) {
// //             cout << "ERROR: couldn't set pin direction ... exiting" << endl;
// //             exit(1);
// //         }
// //
// //         if (gpioSetPullUpDown(pin, PI_PUD_UP) != 0) {
// //             cout << "ERROR: couldn't set up/down resistor ... exiting" << endl;
// //             exit(1);
// //         }
// //     }
// //
// //     void toggle_direction(bool dir){
// //         gpioWrite(dir_pin, dir ? 1 : 0);
// //     }
// // #else
//     // void io_init(void){}
//     // void toggle_direction(bool dir){
//     //     setRTS(dir);
//     //     setDTR(dir);
//     // }
// // #endif
// };
