#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""OK. This is a sample Scribus database connection with Python HOWTO.

DESCRIPTION:
I've got questions about DB and Scribus in my personal mailbox 3-4
times in a month. So this is an common answer for asking people.

Even through I'm an Oracle user/developer I choose MySQL for this
example, because of its presence in the Linux distributions and
hosting availability too.
But the DB server doesn't matter due the PEP 249 - standard DB interface
(http://www.python.org/peps/pep-0249.html).
There are various modules for database accessing:
http://www.python.org/topics/database/modules.html

Anyway - this script provides connection to the database server by the
specified values in the hostname, dbname, username, and password variables.
Then it checks the system for table names in the specified databases
and it displays it in the new document finally. Easy and understandable.


CONTACT:
email : petr@yarpen.cz


LICENSE:
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
"""

import sys

# environment checking
try:
    import scribus
except ImportError:
    print "This script only runs from within Scribus."
    sys.exit(1)

try:
    import MySQLdb
except ImportError:
    print "You must have 'MySQLdb' installed."
    sys.exit(1)


# connection parameters
hostname = 'server.foo.org'
dbname = 'name'
username = 'username'
password = 'password'

# connection to the network wide server would be time consuming. So get the hint to the user.
scribus.statusMessage('Connecting to the ' + hostname + ' server. Be patient, please...')

# Database related issues
try:
    conn = MySQLdb.connect(passwd=password, db=dbname, host=hostname, user=username)
except:
	scribus.messageBox('DB connection example', 'Connection error. You should specify your login in the script')
	sys.exit(1)

cur = conn.cursor()
# get the list of the databases
# it's like 'select * from dba_tables' in Oracle
count = cur.execute('show tables')
# formating the output
result = str(count) + ' table(s) in the ' + dbname + ' database.\n\n'
for i in cur.fetchall():
    result = result + i[0] + '\n'

# Scribus presentation part
scribus.newDoc(scribus.PAPER_A5, (10, 10, 20, 20), scribus.PORTRAIT, 1, scribus.UNIT_POINTS, scribus.NOFACINGPAGES, scribus.FIRSTPAGERIGHT)
txtName = scribus.createText(10, 10, 200, 200)
scribus.setText(result, txtName)

scribus.statusMessage('Script done.')
