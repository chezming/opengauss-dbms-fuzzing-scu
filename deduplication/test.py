import os
import gdb

BTInfo = gdb.execute("bt 100", to_string=True)

with open("/home/wx/core/result_test", "w+") as fd:
	fd.write(BTInfo);

gdb.execute("q")
