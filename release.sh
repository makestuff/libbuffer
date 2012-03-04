#
# Copyright (C) 2009-2012 Chris McClelland
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# Helper script for building the binary distribution. It's unlikely you'll need
# this unless you're forking the project.
#
#!/bin/bash
export LIB=libbuffer
export DATE=$(date +%Y%m%d)
rm -rf ${LIB}-${DATE}
mkdir ${LIB}-${DATE}

# Linux x86_64 binaries
mkdir -p ${LIB}-${DATE}/linux.x86_64/rel
cp -rp linux.x86_64/rel/*.so ${LIB}-${DATE}/linux.x86_64/rel/
mkdir -p ${LIB}-${DATE}/linux.x86_64/dbg
cp -rp linux.x86_64/dbg/*.so ${LIB}-${DATE}/linux.x86_64/dbg/

# Linux i686 binaries
mkdir -p ${LIB}-${DATE}/linux.i686/rel
cp -rp linux.i686/rel/*.so ${LIB}-${DATE}/linux.i686/rel/
mkdir -p ${LIB}-${DATE}/linux.i686/dbg
cp -rp linux.i686/dbg/*.so ${LIB}-${DATE}/linux.i686/dbg/

# Linux armel binaries
mkdir -p ${LIB}-${DATE}/linux.armel/rel
cp -rp linux.armel/rel/*.so ${LIB}-${DATE}/linux.armel/rel/
mkdir -p ${LIB}-${DATE}/linux.armel/dbg
cp -rp linux.armel/dbg/*.so ${LIB}-${DATE}/linux.armel/dbg/

# Linux ppc binaries
mkdir -p ${LIB}-${DATE}/linux.ppc/rel
cp -rp linux.ppc/rel/*.so ${LIB}-${DATE}/linux.ppc/rel/
mkdir -p ${LIB}-${DATE}/linux.ppc/dbg
cp -rp linux.ppc/dbg/*.so ${LIB}-${DATE}/linux.ppc/dbg/

# MacOS binaries
mkdir -p ${LIB}-${DATE}/darwin/rel
cp -rp darwin/rel/*.dylib ${LIB}-${DATE}/darwin/rel/
mkdir -p ${LIB}-${DATE}/darwin/dbg
cp -rp darwin/dbg/*.dylib ${LIB}-${DATE}/darwin/dbg/

# Windows binaries
mkdir -p ${LIB}-${DATE}/win32/rel
cp -rp win32/rel/*.dll ${LIB}-${DATE}/win32/rel/
cp -rp win32/rel/*.lib ${LIB}-${DATE}/win32/rel/
cp -rp win32/rel/*.pdb ${LIB}-${DATE}/win32/rel/
mkdir -p ${LIB}-${DATE}/win32/dbg
cp -rp win32/dbg/*.dll ${LIB}-${DATE}/win32/dbg/
cp -rp win32/dbg/*.lib ${LIB}-${DATE}/win32/dbg/
cp -rp win32/dbg/*.pdb ${LIB}-${DATE}/win32/dbg/

# Headers
cp -rp ../../common/makestuff.h ${LIB}-${DATE}/
cp -rp ${LIB}.h ${LIB}-${DATE}/

cp -p COPYING COPYING.LESSER ${LIB}-${DATE}/
cat > ${LIB}-${DATE}/README <<EOF
LibBuffer Binary Distribution

LibBuffer is a library for manipulating dynamic binary buffers, and loading and saving them as
binary or Intel hex files.

Overview here: http://www.makestuff.eu/wordpress/?page_id=1688
Source code here: https://github.com/makestuff/libbuffer
API docs here: http://bit.ly/buffer-api
EOF

# Package it up
tar zcf ${LIB}-${DATE}.tar.gz ${LIB}-${DATE}
rm -rf ${LIB}-${DATE}
cp -p ${LIB}-${DATE}.tar.gz /mnt/ukfsn/bin/
