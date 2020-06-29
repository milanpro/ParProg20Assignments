#!/usr/bin/env python3
# -*- coding: UTF-8 -*-

"""
ParProg/ST20/T3.3/v1

	$ color() (set -o pipefail; "$@" 2>&1>&3 | sed $'s,.*,\e[31m&\e[m,' >&2) 3>&1
	$ make
	$ color python3 path/to/validator.py

"""

import operator
import os
import subprocess
import sys

#-------------------------------------------------------------------------------

Test_cwd = None

output_lines = []

def save_log(msg="", newLine=True, okay=False):
	output_lines.append((okay, "%s\n" % msg if newLine else msg))

def print_log(msg="", newLine=True, okay=False):
	f = sys.stdout if okay else sys.stderr
	f.write("%s\n" % msg if newLine else msg)
	f.flush()

log = save_log

def in_cwd(filename):
	if Test_cwd is None:
		return filename
	return os.path.join(Test_cwd, filename)

def silent_atoi(s):
	try:
		return int(s)
	except ValueError:
		return None

#-------------------------------------------------------------------------------

class Test(object):

	def exec_(self, input_text, *cmd_and_args):
		p = subprocess.Popen(cmd_and_args, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd=Test_cwd)
		return tuple(map(lambda s: s.strip(), p.communicate(input=input_text)[:2])) + (p.returncode, )

	def run_prog(self, *cmd_and_args):
		return self.exec_("", *cmd_and_args)[0]

	def test(self):
		self.run()
		okay = self.okay()
		log(self.what(), okay=okay)
		return okay

	def run(self):
		raise NotImplementedError

	def what(self):
		raise NotImplementedError

	def okay(self):
		raise NotImplementedError


class EmptyTestLine(Test):

	def __init__(self, text=""):
		self.text = text

	def run(self):
		pass

	def what(self):
		return self.text

	def okay(self):
		return True


class TestGroup(Test):
	"""A group of tests, where all tests are executed"""

	def __init__(self, *tests):
		self.tests = tests
		self.is_okay = False
 
	def test(self):
		self.is_okay = all([t.test() for t in self.tests])
		return self.is_okay

	def okay(self):
		return self.is_okay


class AbortingTestGroup(TestGroup):
	"""A group of tests, where the first failed test stops the entire group"""

	def test(self):
		self.is_okay = all(t.test() for t in self.tests)
		return self.is_okay


class ReturnCodeTest(Test):

	def __init__(self, progname, args=None, retcode=0, showstdout=False, showstderr=False, inputText="", truncateLines=None):
		self.progname = progname
		self.inputText = inputText
		self.cmdline = [progname] + list(map(str, args or []))
		self.expected = retcode
		self.exception = None
		self.showstdout = showstdout
		self.showstderr = showstderr
		self.output = None
		self.truncateLines = truncateLines
		self.stdout = ""
		self.stderr = ""

	def _truncate(self, text):
		# Hoping for PEP 572 to fix this mess
		#    https://www.python.org/dev/peps/pep-0572/
		# Okay, hoping for submit-exec to support Python 3.8

		lines = text.split("\n")
		if self.truncateLines is None or len(lines) <= self.truncateLines:
			return text

		return "\n".join(lines[:self.truncateLines] + ["", "[OUTPUT TRUNCATED]"])

	def _get_output(self, tpl):
		self.stdout = tpl[0].decode()
		self.stderr = tpl[1].decode()
		return tpl[2]

	def run(self):
		try:
			self.output = self._get_output(self.exec_(self.inputText, *self.cmdline))
		except Exception as e:
			self.exception = e

	def _s(self, a):
		return " ".join(map(str, a))

	def _formatCall(self):
		if self.inputText:
			return "echo -ne %r | %s" % (self.inputText, self._s(self.cmdline))
		return self._s(self.cmdline)

	def what(self):
		return "$ %s #- exited with %s (expected %d)%s%s" % (
					self._formatCall(),
					str(self.output) if self.exception is None else repr(str(self.exception)),
					self.expected,
					"" if not (self.showstdout and self.stdout) else ("\n%s" % self._truncate(self.stdout)),
					"" if not (self.showstderr and self.stderr) else ("\n%s" % self._truncate(self.stderr)),
				)

	def okay(self):
		return self.output == self.expected


class ExecutionTest(ReturnCodeTest):
	"""Run a programm and compare the lines of stdout to a list of expected output."""

	def __init__(self, progname, args, expected, compare=operator.eq, key=lambda x: x, inputText = "", showstderr=False):
		super(ExecutionTest, self).__init__(progname, args, expected, inputText=inputText, showstderr=showstderr)
		self.compare = compare
		self.key = key

	def _get_output(self, tpl):
		super(ExecutionTest, self)._get_output(tpl)
		return self.stdout.split("\n")

	def what(self):
		_r = repr
		return "$ " + self._formatCall() + "\n" + \
				("" if not (self.showstderr and self.stderr) else ("%s\n" % self.stderr)) + \
				(("received: " + _r(self.output))
						if self.exception is None
						else str(self.exception)) + "\n" + \
			   "expected: " + _r(self.expected)

	def okay(self):
		return self.exception is None and self.compare(self.key(self.output), self.key(self.expected))

#-------------------------------------------------------------------------------

import codecs

class CreateFileJob(Test):

	def __init__(self, filename, contents):
		self.filename = filename
		self.contents = contents

	def run(self):
		with open(in_cwd(self.filename), "wb") as f:
			f.write(self.contents)

	def okay(self):
		return True

	def what(self):
		return "Preparing %s" % self.filename
		

class ColorfilterOutputTest(Test):

	filename = "output.bmp"
	BMP_FILE_HEADER_SIZE = 14
	BMP_DIB5_HEADER_SIZE = 124

	def run(self):
		self.exists = True
		try:
			self.contents = open(in_cwd(self.filename), "rb").read()
		except FileNotFoundError:
			self.exists = False

	def okay(self):
		return self.exists

	def what(self):
		if not self.exists:
			return "%s not found" % (self.filename)

	def pixeldata(self, data):
		return data[self.BMP_FILE_HEADER_SIZE + self.BMP_DIB5_HEADER_SIZE:]


class ColorfilterEqualTest(ColorfilterOutputTest):

	"""Tests if two files are byte-equal (including BMPv5 header)"""

	def __init__(self, expected):
		self.expected = expected

	def hex_dump(self, data, blocksize=4):
		hex_pixels = codecs.encode(self.pixeldata(data), 'hex_codec').decode('ascii')
		return ' '.join([hex_pixels[i:i+blocksize*2] for i in range(0, len(hex_pixels), blocksize*2)])

	def okay(self):
		return super().okay() and self.contents == self.expected

	def what(self):
		s = super().what()
		if s:
			return s
		return "BGRA-pixeldata of %s:\n%s\n%s" % (self.filename, self.hex_dump(self.contents),
				("" if self.okay() else ("expected:\n%s\n" % self.hex_dump(self.expected))))


class ColorfilterGreyTest(ColorfilterOutputTest):
	
	"""Tests if output is grey-scaled input, i.e.:
		* All channels have the same value Y assigned
		* The grey value Y is min{R,G,B} <= Y <= max{R,G,B}
	"""

	def __init__(self, colordata):
		self.colordata = colordata
		self.failnr = None

	def is_grey(self, ypixel, cpixel):
		yb, yg, yr, ya = ypixel
		if not (yb == yg == yr):
			return False
		return min(*cpixel) <= yb <= max(*cpixel)

	def iter_pixels(self, data, blocksize=4):
		pixels = self.pixeldata(data)
		return (pixels[i:i+blocksize] for i in range(0, len(pixels), blocksize))

	def okay(self):
		if not super().okay():
			return False
		for nr, (ypx, cpx) in enumerate(zip(self.iter_pixels(self.contents), self.iter_pixels(self.colordata))):
			if not self.is_grey(ypx, cpx):
				self.failnr = nr
				return False
		return True

	def what(self):
		s = super().what()
		if s:
			return s
		if self.failnr is None:
			return "[Okay] All pixels grey"
		return "Pixel #%d is not grey.\nEither not all channels set to the same value, or value exceeds input range." % self.failnr



#-------------------------------------------------------------------------------

import base64

testbmp_org = base64.b64decode("Qk3KAAAAAAAAAIoAAAB8AAAABAAAAAQAAAABACAAAwAAAEAAAADDDgAAww4AAAAAAAAAAAAAAAD/AAD/AAD/AAAAAAAA/0JHUnOPwvUoUbgeFR6F6wEzMzMTZmZmJmZmZgaZmZkJPQrXAyhcjzIAAAAAAAAAAAAAAAAEAAAAAAAAAAAAAAAAAAAAKxmu/ysZrv8rGa7//////ysZrv8MZN3/DGTd/wCo9v8rGa7/DGTd/wxk3f8AqPb//////wCo9v8AqPb/AKj2/w==")
testbmp_neg = base64.b64decode("Qk3KAAAAAAAAAIoAAAB8AAAABAAAAAQAAAABACAAAwAAAEAAAADDDgAAww4AAAAAAAAAAAAAAAD/AAD/AAD/AAAAAAAA/0JHUnOPwvUoUbgeFR6F6wEzMzMTZmZmJmZmZgaZmZkJPQrXAyhcjzIAAAAAAAAAAAAAAAAEAAAAAAAAAAAAAAAAAAAA1OZR/9TmUf/U5lH/AAAA/9TmUf/zmyL/85si//9XCf/U5lH/85si//ObIv//Vwn/AAAA//9XCf//Vwn//1cJ/w==")

def main(cd=None):
	tests = AbortingTestGroup(
		CreateFileJob("test.bmp", testbmp_org),
		AbortingTestGroup(
			ReturnCodeTest("./colorfilter", ["1", "test.bmp"]),
			ColorfilterEqualTest(testbmp_neg),
		),
		AbortingTestGroup(
			ReturnCodeTest("./colorfilter", ["2", "test.bmp"]),
			ColorfilterGreyTest(testbmp_org),
			
		),
	)

	return tests.test()

#-------------------------------------------------------------------------------

def validate(job):
	global Test_cwd
	Test_cwd = job.working_dir

	job.run_make(mandatory=True)
	valid = main()
	output = "\n".join(msg for _, msg in output_lines)
	if valid:
		job.send_pass_result(output)
	else:
		job.send_fail_result(output)

if __name__ == "__main__":
	log = print_log
	sys.exit(0 if main() else 1)

