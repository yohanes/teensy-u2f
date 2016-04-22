teensy-u2f
==========

U2F implementation for Teensy LC. 

This implementation is simple, works, but a bit insecure in the key handle generation part and user presence check. 

The key handle is generated from private key  XOR-ed  with a simple fixed key (note: key handle generation is outside of U2F implementation scope). Attacker that knows the fixed key (or able to deduce it from multiple registrations request) can get the private key sign any authentication request although in practice its not that easy to perform this attack.

Because there is no user button in Teensy LC, I didn't actually implement any button handling logic for 'user-presence' check. On first request this implementation will assume button is not pressed, and on next request it will assume the user has pressed the button. When logging in to a website, you may need to unplug and replug the Teensy LC.

For the ECDSA key generation and signing this implementation uses the micro-ecc library:

<https://github.com/kmackay/micro-ecc>


License
-------

See LICENSE.txt
