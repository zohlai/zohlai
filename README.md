## Zohlai
[![Build Status](https://travis-ci.org/zohlai/zohlai.svg?branch=master)](https://travis-ci.org/zohlai/zohlai)

Zohlai is a set of services for IRC networks designed for large IRC networks with high
scalability requirements.  It is relatively mature software, with some code and design
derived from another package called Shrike, and is a fork of Atheme IRC services.

Zohlai's behavior is tunable using modules and a highly detailed configuration file.
Almost all behavior can be changed at deployment time just by editing the configuration.

If you are running this code from Git, you should read `GIT-Access` for
instructions on how to fully check out the zohlai tree, as it is spread
across a few repositories.

## Building

Whatever you do, make sure you do *not* install Zohlai into the same location
as the source. Zohlai will default to installing in `$HOME/zohlai`, so make
sure you plan accordingly for this.

    $ git submodule update --init
    $ ./configure
    $ make
    $ make install

If you're still lost, read [INSTALL](INSTALL) or [GIT-Access](GIT-Access) for hints.

## Contact us

 * [GitHub](http://www.github.com/zohlai/zohlai)
 * [IRC](irc://chat.freenode.net/#zohlai)
