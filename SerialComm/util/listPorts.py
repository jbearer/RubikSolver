#
# Script to list names of all serial ports.
# Source http://eli.thegreenplace.net/2009/07/31/listing-all-serial-ports-on-windows-with-python
# Modified by Jeb Bearer
#

import _winreg as winreg
import itertools
import re

def full_port_name(portname):
    """ Given a port-name (of the form COM7,
        COM12, CNCA0, etc.) returns a full
        name suitable for opening with the
        Serial class.
    """
    m = re.match('^COM(\d+)$', portname)
    if m and int(m.group(1)) < 10:
        return portname
    return '\\\\.\\' + portname

def enumerate_serial_ports():
    """ Uses the Win32 registry to return an
        iterator of serial (COM) ports
        existing on this computer.
    """
    path = 'HARDWARE\\DEVICEMAP\\SERIALCOMM'
    try:
        key = winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, path)
    except WindowsError:
        raise IterationError

    for i in itertools.count():
        try:
            val = winreg.EnumValue(key, i)
            yield full_port_name(str(val[1]))
        except EnvironmentError:
            break

if __name__ == '__main__':
    for port in enumerate_serial_ports():
        print port