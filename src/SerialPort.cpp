#include <mv/SerialPort.hpp>
#include <sys/ioctl.h>  // dtr or rts
#include <fcntl.h>      // open
#include <termios.h>    // serial
#include <string.h>     // memset
#include <iostream>
// #include <termcolor/termcolor.hpp>


Serial::Serial(): fd(0) {}
Serial::~Serial(){close();}

bool Serial::open(const std::string& port){
    struct termios t;

    fd = ::open(port.c_str(), O_RDWR|O_NOCTTY|O_NONBLOCK);
    if(fd < 0){
        perror("Error opening serial port");
        return false;
    }

    memset(&t, 0, sizeof(t)); // clear struct for new port settings

    t.c_cflag = B1000000 | CS8 | CLOCAL | CREAD;
    t.c_iflag = IGNPAR;
    t.c_oflag      = 0;
    t.c_lflag      = 0;
    t.c_cc[VTIME]  = 0; // 10th of second
    t.c_cc[VMIN]   = 0;

    // clean the buffer and activate the settings for the port
    tcflush(fd, TCIFLUSH);
    if (tcsetattr(fd, TCSANOW, &t) < 0) perror("*** Couldn't set port attribute");

    // if (tcgetattr(fd, &t) < 0) perror("Couldn't get port attribute");
    // printf(">> %u %u \n", cfgetispeed(&t), cfgetospeed(&t));

    set_dir(DD_READ);
    msleep(100);
    set_dir(DD_WRITE);

    return true;
}

void Serial::close(){
    if (fd > 0) ::close(fd);
}

int Serial::write(const packet& pkt){
    set_dir(DD_WRITE);
    int ret = ::write(fd, pkt.data(), pkt.size());
    // for (const auto& p: buff) ::write(fd, (void*)&p, 1);
    flush_output();
    msleep(1);
    set_dir(DD_READ);
    return ret;
}

int Serial::read(){
    // smallest packet is 6 bytes
    int num = available();
    if (num < 6) return 0;

    buffer.fill(0);
    size_t remain = num;
    while(remain > 0){
        remain -= ::read(fd, buffer.data() + remain, remain);
    }

    // TODO: check error code and return it if error
    return num;
}

packet Serial::buffer2packet(int num, int offset){
    auto pkt = packet(num);
    std::copy(buffer.begin(), buffer.begin() + offset, pkt.begin());
    return pkt;
}

status_t Serial::decode(){
    status_t ret;
    // resp: [s,s,id,len,err, ... ,chksum]
    for (int i = 0; i < buffer.size() - 2; ++i) {
        if (buffer[i] == 0xff && buffer[i+1] == 0xff) {
            ret.id = buffer[i+2];
            ret.error = buffer[i+4];
            for (int j=0; j < buffer[i+3]-2; ++j) ret.params.push_back(buffer[i+5+j]);
            break;
        }
    }

    return ret;
}

void Serial::flush_input(){
    tcflush(fd, TCIFLUSH);
}

void Serial::flush_output(){
    tcflush(fd, TCOFLUSH);
}

void Serial::flush_all(){
    tcflush(fd, TCIOFLUSH);
}

int Serial::available(){
    int bytes_available;
    ioctl(fd, FIONREAD, &bytes_available);
    return bytes_available;
}

void Serial::set_dir(bool enabled){
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
