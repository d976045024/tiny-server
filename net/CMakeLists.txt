set(net_SRCS
  channel.cc
  eventloop.cc
  poller.cc
  poll_poller.cc)

add_library(server_net ${net_SRCS})
target_link_libraries(server_net server_base)