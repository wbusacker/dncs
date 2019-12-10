# Distributed Network Computer System

The DNCS is an ethernet communication backend that allows for multiple computers to be joined in a group and relay information between all machines

## Modes

Groups perform three main operations

- Joining
- Synchronizing
- Data Sharing

### Joining

A computer joins a network by sending a request to any one computer in the group. The request contains a name the computer wants to be known as in the group. The contacted computer then responds to the request with the IP address it detected as well as its own name. It also sends a command to all other computers in the group to do the same.

If a computer is starting a network, when it is contacted by the first joinee, it responds similar to above but lets the know joinee that they are establishing a group. The joinee then also sends back a join response to the other computer as to populate its own global facing 