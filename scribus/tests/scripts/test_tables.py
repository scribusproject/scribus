#!/usr/bin/env python

"""
Test script for tables.

Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.

To add a new test, simply add a new test method named 'test_mytest' to the
TableTests class, and the test will be runned automatically.

Use check() to check a condition and fail(msg) to manually fail a test. The
tests are runned in a "fail fast" fashion; on failure, the test method will
stop executing and testing move on to the next test method.
"""

from scribus import *
from traceback import print_exc
from sys import stdout
from inspect import getmembers, ismethod
from time import time

class TableTests:
    def __init__(self):
        # The default border width of tables.
        self.defaultBorderWidth = 1.0

    """ Tests for tables """
    def test_construction(self):
        """ Test for createTable(...) """
        # Test valid construction.
        table = createTable(50, 50, 90, 90, 3, 3)
        check(objectExists(table))
        check(getTableColumns(table) == 3)
        check(getTableRows(table) == 3)
        check(float_equal(getTableColumnWidth(0, table), (90 - self.defaultBorderWidth)/3))
        check(float_equal(getTableColumnWidth(1, table), (90 - self.defaultBorderWidth)/3))
        check(float_equal(getTableColumnWidth(2, table), (90 - self.defaultBorderWidth)/3))
        check(float_equal(getTableRowHeight(0, table), (90 - self.defaultBorderWidth)/3))
        check(float_equal(getTableRowHeight(1, table), (90 - self.defaultBorderWidth)/3))
        check(float_equal(getTableRowHeight(2, table), (90 - self.defaultBorderWidth)/3))
        deleteObject(table)

        # Test invalid construction.
        bad_args = [(-1, 3), (3, -1), (0, 3), (3, 0)]
        for args in bad_args:
            try:
                table = createTable(50, 50, 90, 90, args[0], args[1]);
                fail('Expected ValueError, numRows=%i, numColumns=%i' % (args[0], args[1]))
            except ValueError:
                pass # Expected.
            finally:
                if objectExists(table):
                    deleteObject(table)

    def test_insert_rows(self):
        """ Test for insertTableRows(...) """
        # TODO: - Test individual cell rects
        #       - Test that rows were inserted in the right place.

        # Test valid API usage.
        for index in range(3):
            # Insert one row.
            table = createTable(50, 50, 90, 90, 3, 3)
            insertTableRows(index, 1, table)
            check(getTableRows(table) == 4)
            deleteObject(table)

            # Insert three rows.
            table = createTable(50, 50, 90, 90, 3, 3)
            insertTableRows(index, 3, table)
            check(getTableRows(table) == 6)
            deleteObject(table)

        # Test invalid API usage.
        for invalidIndex in [-1, 4]:
            try:
                table = createTable(50, 50, 90, 90, 3, 3)
                insertTableRows(invalidIndex, 1, table)
                fail('Expected ValueError, index=%i' % invalidIndex)
            except ValueError:
                pass # Expected.
            finally:
                if objectExists(table):
                    deleteObject(table)

    def test_remove_rows(self):
        """ Test for removeTableRows(...) """
        # TODO: - Test individual cell rects
        #       - Test that rows were removed from the right place.

        # Test valid API usage.
        for index in range(3):
            # Remove one row at a valid index.
            table = createTable(50, 50, 90, 90, 3, 3)
            removeTableRows(index, 1, table)
            check(getTableRows(table) == 2)
            deleteObject(table)

        # Remove two first rows.
        table = createTable(50, 50, 90, 90, 3, 3)
        removeTableRows(0, 2, table)
        check(getTableRows(table) == 1)
        deleteObject(table)

        # Remove two last rows.
        table = createTable(50, 50, 90, 90, 3, 3)
        removeTableRows(1, 2, table)
        check(getTableRows(table) == 1)
        deleteObject(table)

        # Test invalid API usage.
        bad_args = [(-1, 1), (1, -1), (0, 3), (0, 4), (1, 3)]
        for args in bad_args:
            try:
                table = createTable(50, 50, 90, 90, 3, 3)
                removeTableRows(args[0], args[1], table)
                fail('Expected ValueError, index=%i, numRows=%i' % (args[0], args[1]))
            except ValueError:
                pass # Expected.
            finally:
                if objectExists(table):
                    deleteObject(table)

    def test_insert_columns(self):
        """ Test for insertTableColumns(...) """
        # TODO: - Test individual cell rects
        #       - Test that columns were inserted in the right place.

        # Test valid API usage.
        for index in range(3):
            # Insert one column.
            table = createTable(50, 50, 90, 90, 3, 3)
            insertTableColumns(index, 1, table)
            check(getTableColumns(table) == 4)
            deleteObject(table)

            # Insert three columns.
            table = createTable(50, 50, 90, 90, 3, 3)
            insertTableColumns(index, 3, table)
            check(getTableColumns(table) == 6)
            deleteObject(table)

        # Test invalid API usage.
        for invalidIndex in [-1, 4]:
            try:
                table = createTable(50, 50, 90, 90, 3, 3)
                insertTableColumns(invalidIndex, 1, table)
                fail('Expected ValueError, index=%i' % invalidIndex)
            except ValueError:
                pass # Expected.
            finally:
                if objectExists(table):
                    deleteObject(table)

    def test_remove_columns(self):
        """ Test for removeTableColumns(...) """
        # TODO: - Test individual cell rects
        #       - Test that columns were removed from the right place.

        # Test valid API usage.
        for index in range(3):
            # Remove one column at a valid index.
            table = createTable(50, 50, 90, 90, 3, 3)
            removeTableColumns(index, 1, table)
            check(getTableColumns(table) == 2)
            deleteObject(table)

        # Remove two first columns.
        table = createTable(50, 50, 90, 90, 3, 3)
        removeTableColumns(0, 2, table)
        check(getTableColumns(table) == 1)
        deleteObject(table)

        # Remove two last columns.
        table = createTable(50, 50, 90, 90, 3, 3)
        removeTableColumns(1, 2, table)
        check(getTableColumns(table) == 1)
        deleteObject(table)

        # Test invalid API usage.
        bad_args = [(-1, 1), (1, -1), (0, 3), (0, 4), (1, 3)]
        for args in bad_args:
            try:
                table = createTable(50, 50, 90, 90, 3, 3)
                removeTableColumns(args[0], args[1], table)
                fail('Expected ValueError, index=%i, numColumns=%i' % (args[0], args[1]))
            except ValueError:
                pass # Expected.
            finally:
                if objectExists(table):
                    deleteObject(table)

    def test_resize_table_row(self):
        """ Test for resizeTableRow(...) """
        # TODO: Test individual cell rects.

        # Test valid API usage.
        table1 = createTable(50, 50, 90, 90, 3, 3)
        for row in range(2):
            resizeTableRow(row, 20, table1)
            check(getTableRowHeight(row, table1) == 20)
        deleteObject(table1)

        # Test invalid API usage.
        bad_args = [(-1, 10), (3, 10), (-1, 0), (0, 0)]
        for args in bad_args:
            try:
                table2 = createTable(50, 50, 90, 90, 3, 3)
                resizeTableRow(args[0], args[1], table2)
                fail('Expected ValueError, row=%i, height=%i' % (args[0], args[1]))
            except ValueError:
                pass # Expected.
            finally:
                if objectExists(table2):
                    deleteObject(table2)

    def test_resize_table_column(self):
        """ Test for resizeTableColumn(...) """
        # TODO: Test individual cell rects.

        # Test valid API usage.
        table1 = createTable(50, 50, 90, 90, 3, 3)
        for col in range(2):
            resizeTableColumn(col, 20, table1)
            check(getTableColumnWidth(col, table1) == 20)
        deleteObject(table1)

        # Test invalid API usage.
        bad_args = [(-1, 10), (3, 10), (-1, 0), (0, 0)]
        for args in bad_args:
            try:
                table2 = createTable(50, 50, 90, 90, 3, 3)
                resizeTableColumn(args[0], args[1], table2)
                fail('Expected ValueError, col=%i, height=%i' % (args[0], args[1]))
            except ValueError:
                pass # Expected.
            finally:
                if objectExists(table2):
                    deleteObject(table2)

    def test_merge_cells(self):
        """ Test for mergeTableCells(...) """
        # Test valid API usage.

        # Create a 7x7 table.
        table1 = createTable(50, 50, 90, 90, 7, 7)

        # Merge the area (2,2 3x3).
        mergeTableCells(2, 2, 3, 3, table1)
        # We expect getCellRowSpan(...) == getCellColumnSpan(...) == 3 inside (2,2 3x3).
        area1 = set([
                (2, 2), (2, 3), (2, 4),
                (3, 2), (3, 3), (3, 4),
                (4, 2), (4, 3), (4, 4)])
        self.check_spans(area1, 3, 3, table1)

        # Merge the area (1,2 1x2), which overlaps the previous area at the top.
        mergeTableCells(1, 2, 2, 1, table1);
        # We expect getCellRowSpan(...) == 4, getCellColumnSpan(...) == 3 inside (1,2 3x4).
        area2 = set([
                (1, 2), (1, 3), (1, 4),
                (2, 2), (2, 3), (2, 4),
                (3, 2), (3, 3), (3, 4),
                (4, 2), (4, 3), (4, 4)])
        self.check_spans(area2, 4, 3, table1)

        # Merge the area (4,4 2x1), which overlaps the previous area on the right.
        mergeTableCells(4, 4, 1, 2, table1);
        # We expect getCellRowSpan(...) == getCellColumnSpan(...) == 4 inside (1,2 4x4).
        area3 = set([
                (1, 2), (1, 3), (1, 4), (1, 5),
                (2, 2), (2, 3), (2, 4), (2, 5),
                (3, 2), (3, 3), (3, 4), (3, 5),
                (4, 2), (4, 3), (4, 4), (4, 5)])
        self.check_spans(area3, 4, 4, table1)

        # Merge the area (4,3 1x2), which overlaps the previous area at the bottom.
        mergeTableCells(4, 3, 2, 1, table1);
        # We expect getCellRowSpan(...) == 5, getCellColumnSpan(...) == 4 inside (1,2 4x5).
        area4 = set([
                (1, 2), (1, 3), (1, 4), (1, 5),
                (2, 2), (2, 3), (2, 4), (2, 5),
                (3, 2), (3, 3), (3, 4), (3, 5),
                (4, 2), (4, 3), (4, 4), (4, 5),
                (5, 2), (5, 3), (5, 4), (5, 5)])
        self.check_spans(area4, 5, 4, table1)

        # Merge the area (4,1 2x1), which overlaps the previous area on the left.
        mergeTableCells(4, 1, 1, 2, table1);
        # We expect getCellRowSpan(...) == getCellColumnSpan(...) == 5 inside (1,1 5x5).
        area5 = set([
                (1, 1), (1, 2), (1, 3), (1, 4), (1, 5),
                (2, 1), (2, 2), (2, 3), (2, 4), (2, 5),
                (3, 1), (3, 2), (3, 3), (3, 4), (3, 5),
                (4, 1), (4, 2), (4, 3), (4, 4), (4, 5),
                (5, 1), (5, 2), (5, 3), (5, 4), (5, 5)])
        self.check_spans(area5, 5, 5, table1)

        deleteObject(table1)

        # Test invalid API usage.
        bad_args = [(-1, 1, 2, 2), (1, -1, 2, 2), (1, 1, 0, 2), (1, 1, 2, 0),
                    (1, 1, -2, 2), (1, 1, 2, -2), (-1, -1, -2, -2), (1, 1, 3, 3)]
        for args in bad_args:
            try:
                table2 = createTable(50, 50, 90, 90, 3, 3)
                mergeTableCells(args[0], args[1], args[2], args[3], table2)
                fail('Expected ValueError, row=%i, col=%i, numRows=%i, numColumns=%i' %
                        (args[0], args[1], args[2], args[3]))
            except ValueError:
                pass # Expected.
            finally:
                if objectExists(table2):
                    deleteObject(table2)

    def check_spans(self, area, expected_row_span, expected_column_span, table):
        """
        Utility method for cell span checking.

        Given a table and an area of cells as a set of (row, column) tuples, checks
        that getCellRowSpan(...) returns expected_row_span and getCellColumnSpan(...)
        returns expected_column_span for every cell in the area, and that they both
        return 1 for each cell in the table that is outside the area.
        """
        for row in range(getTableRows(table)):
            for col in range(getTableColumns(table)):
                if (row, col) in area:
                    check(getCellRowSpan(row, col, table) == expected_row_span)
                    check(getCellColumnSpan(row, col, table) == expected_column_span)
                else:
                    check(getCellRowSpan(row, col, table) == 1)
                    check(getCellColumnSpan(row, col, table) == 1)

#
# Test "framework" code below.
#
class TestFailure(Exception):
    """ Raised by fail() """
    def __init__(self, msg):
        self.msg = msg
    def __str__(self):
        return repr(self.msg)

def float_equal(a, b):
    return abs(a - b) < 1e-6

def check(condition):
    """ Fails test if condition is false """
    if not condition:
        fail('Check failed')

def fail(msg):
    """ Fails test with msg """
    raise TestFailure(msg);

def is_test_method(obj):
    """ Returns True if obj is a test method """
    return ismethod(obj) and obj.__name__.startswith('test_')

if __name__ == '__main__':
    print 'Running table tests...'
    tests = TableTests()
    methods = getmembers(tests, is_test_method)
    ntests = len(methods)
    nfailed = 0
    total_time = 0
    for testnr, (name, method) in enumerate(methods):
        print '\t%i/%i: %s()%s' % (testnr + 1, ntests, name, '.' * (30 - len(name))),
        try:
            start_time = time()
            method()
            test_time = time() - start_time
            total_time += test_time
        except:
            print 'Failed'
            print_exc(file=stdout)
            nfailed += 1
        else:
            print 'Passed  %.3f s' % round(test_time, 3)
    print '%i%% passed, %i tests failed out of %i' % (int(round((float(ntests - nfailed)/ntests)*100)), nfailed, ntests)
    print 'total test time = %.3f s' % round(total_time, 3)
