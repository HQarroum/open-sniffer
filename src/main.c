#include <sync_reader.h>
#include <io.h>
#include <osi.h>
#include <app.h>

#include <dissectors/ethernet/ethernet.h>
#include <dissectors/ip/ip.h>
#include <dissectors/arp/arp.h>
#include <dissectors/udp/udp.h>
#include <dissectors/icmp/icmp.h>
#include <dissectors/tcp/tcp.h>
#include <dissectors/http/http.h>
#include <dissectors/dns/dns.h>

#define PACKET_SIZE 1024

// The OSI stack used in this program.
static osi_stack_t osi_stack;

// Indicates whether the app is running.
// Can be modified by signal handler.
static volatile sig_atomic_t started;

/**
 * \brief Catches SIGINT signals.
 */
void sighandler(int sig)
{
  if (sig == SIGINT) {
    started = 0;
  }
}

/**
 * \brief Called back as soon as a new packet has been read
 * by the `sync_reader_t`.
 */
void on_new_packet(_UNUSED_ const sync_reader_t* reader, const packet_t* packet)
{
  osi_dump_packet(&osi_stack, packet);
}

int main(void)
{
  sync_reader_t reader;
  int		raw_socket;
  
  // Intercepting a SIGINT.
  signal(SIGINT, sighandler);

  // Initializing the OSI stack.
  osi_stack = osi_stack_new();
  
  // Loading the dissectors.
  osi_register_dissector(&osi_stack, ethernet_dissector_new());
  osi_register_dissector(&osi_stack, ip_dissector_new());
  osi_register_dissector(&osi_stack, arp_dissector_new());
  osi_register_dissector(&osi_stack, udp_dissector_new());
  osi_register_dissector(&osi_stack, dns_dissector_new());
  osi_register_dissector(&osi_stack, icmp_dissector_new());
  osi_register_dissector(&osi_stack, tcp_dissector_new());
  osi_register_dissector(&osi_stack, http_dissector_new());
  
  // Initializing the sync reader.
  raw_socket = make_raw_socket();
  reader     = sync_reader_create(raw_socket);
  sync_reader_add_packet_callback(&reader, on_new_packet);
  
  // Reading on the reader.
  for (started = 1; started;) {
    sync_reader_read(&reader, PACKET_SIZE);
  }
  
  // Clearing allocated resources.
  osi_stack_clear(&osi_stack);
  sync_reader_remove_packet_callback(&reader, on_new_packet);
  xclose(raw_socket);
  return (0);
}
