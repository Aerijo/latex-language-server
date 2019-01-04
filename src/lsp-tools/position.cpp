#include "position.h"

Point::Point() : row(-1), column(-1) {};

Point::Point (f_index row, f_index column) : row(row), column(column) {}

bool Point::hasValue () const {
    return row >= 0 && column >= 0;
}

string Point::toString () const {
    return to_string(row + 1) + ":" + to_string(column + 1);
}

string Point::toExtendedString (const string &fileName) const {
    return fileName + ":" + toString();
}

bool Point::operator == (const Point &that) const {
    return row == that.row && column == that.column;
}

bool Point::operator != (const Point &that) const {
    return row != that.row || column != that.column;
}

bool Point::operator < (const Point &that) const {
    return row < that.row || (row == that.row && column < that.column);
}

bool Point::operator <= (const Point &that) const {
    return row < that.row || (row == that.row && column <= that.column);
}

int Point::compare (const Point &that) const {
    if (row < that.row)
        return -1;
    else if (row > that.row)
        return 1;
    else {
        if (column < that.column)
            return -1;
        else if (column > that.column)
            return 1;
        else
            return 0;
    }
}

void Point::translate (const Point &delta) {
    translate(delta.row, delta.column);
}

void Point::translate (f_index down, f_index across) {
    row += down;
    column += across;
}

void Point::traverse (const Point &delta) {
    traverse(delta.row, delta.column);
}

void Point::traverse (int down, int across) {
    if (down == 0) {
        column += across;
    } else {
        row += down;
        column = across;
    }
}

Range::Range () : Range(Point()) {}

Range::Range (Point point) : Range(point, point) {}

Range::Range (Point start, Point end) : start(start), end(end) {}

bool Range::hasValue () const {
    return start.hasValue();
}

string Range::toString () const {
    return start.toString() + "-" + end.toString();
}

int Range::compare (const Range &that) const {
    int value = start.compare(that.start);
    if (value)
        return value;
    return end.compare(that.end);
}

bool Range::containsPoint (const Point &point, bool exclusive) const {
    return containsPoint(point.row, point.column, exclusive);
}

bool Range::containsPoint (f_index row, f_index column, bool exclusive) const {
    if (row == start.row) {
        if (row == end.row) {
            if (exclusive)
                return start.column < column && column < end.column;
            else
                return start.column <= column && column <= end.column;
        }

        if (exclusive)
            return start.column < column;
        else
            return start.column <= column;
    }

    if (row == end.row) {
        if (exclusive)
            return column < end.column;
        else
            return column <= end.column;
    }

    return start.row < row && row < end.row;
}

bool Range::operator == (const Range &that) const {
    return start == that.start && end == that.end;
}

bool Range::operator != (const Range &that) const {
    return start != that.start || end != that.end;
}

bool Range::intersectsWith (const Range &that, bool exclusive) const {
    if (exclusive)
        return !(end <= that.start || that.end <= start);
    else
        return !(end < that.start || that.end < start);
}

Range Range::merge (const Range &that) const {
    Point merged_start = min(start, that.start);
    Point merged_end = max(end, that.end);

    return Range(merged_start, merged_end);
}

void Range::translate (const Point &delta) {
    translate(delta, delta);
}

void Range::translate (const Point &startDelta, const Point &endDelta) {
    start.translate(startDelta);
    end.translate(endDelta);
}

void Range::traverse (const Point &delta) {
    start.traverse(delta);
    end.traverse(delta);
}

bool Range::coversSameRows (const Range &that) {
    return start.row == that.start.row && end.row == that.end.row;
}

bool Range::containsRange (const Range &that, bool exclusive) const {
    if (exclusive)
        return start < that.start && that.end < end;
    else
        return start <= that.start && that.end <= end;
}

bool Range::intersectsRow (int row) const {
    return start.row <= row && row <= end.row;
}

bool Range::intersectsRowRange(int startRow, int endRow) const {
    return start.row <= endRow && end.row >= startRow;
}
