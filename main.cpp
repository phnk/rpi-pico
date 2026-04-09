#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/tcp.h"
#include "secrets.h"
#include "pico/mutex.h"

#include <string>
#include <cstdlib>

#define TCP_PORT 1234

constexpr uint LED_PIN = 28;

class Program {
public:
    void init() {
        gpio_init(pin_);
        gpio_set_dir(pin_, GPIO_OUT);
        mutex_init(&mutex_);

        printf("Init program\n");
    }

    void init(uint pin) {
        pin_ = pin;
        gpio_init(pin_);
        gpio_set_dir(pin_, GPIO_OUT);
        mutex_init(&mutex_);

        printf("Init program\n");
    }

    void update(const std::string &msg) {
        int val = std::stoi(msg);
        mutex_enter_blocking(&mutex_);
        if (val > 0) {
            interval_ = val;
        }
        mutex_exit(&mutex_);
    }

    void tick() {
        uint32_t now = to_ms_since_boot(get_absolute_time());

        mutex_enter_blocking(&mutex_);
        uint32_t interval = interval_;
        mutex_exit(&mutex_);

        if (now - last_toggle_ > interval) {
            led_state_ = !led_state_;
            gpio_put(pin_, led_state_);
            last_toggle_ = now;
        }
    }

private:
    uint pin_ = 28;
    uint32_t interval_ = 500;
    bool led_state_ = false;
    uint32_t last_toggle_ = 0;
    mutex_t mutex_;
};

static Program prog;

static err_t tcp_recv_cb(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err) {
    if (p == NULL) {
        return ERR_OK;
    }

    std::string msg(static_cast<const char*>(p->payload), p->len);
    prog.update(msg);

    tcp_recved(pcb, p->tot_len);
    pbuf_free(p);
    return ERR_OK;
}

static err_t tcp_accept_cb(void *arg, struct tcp_pcb *newpcb, err_t err) {
    tcp_recv(newpcb, tcp_recv_cb);
    return ERR_OK;
}

int main() {
    stdio_init_all();
    cyw43_arch_init();
    cyw43_arch_enable_sta_mode();
    cyw43_arch_wifi_connect_blocking(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK);

    printf("Connected! IP: %s\n", ip4addr_ntoa(netif_ip4_addr(netif_default)));

    prog.init(28);

    struct tcp_pcb *pcb = tcp_new();
    tcp_bind(pcb, IP_ADDR_ANY, TCP_PORT);
    pcb = tcp_listen(pcb);
    tcp_accept(pcb, tcp_accept_cb);

    while (true) {
        prog.tick();
        sleep_ms(1);
    }

    return 0;
}
