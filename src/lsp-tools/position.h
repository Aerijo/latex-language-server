#ifndef LANGUAGESERVER_TOOLS_TYPES_H
#define LANGUAGESERVER_TOOLS_TYPES_H

#include <string>

#include "definitions.h"

using std::string;

namespace Server {

/*
 * We use signed type so
 * - We can represent invalid Points with negative values
 * - We can easily traverse backwards
 *
 * We also basically assume no overflows will occur. Whoever wants
 * to raise and fix that issue is welcome to.
 */
    struct Point {
        f_index row;
        f_index column;

        Point () = delete;

        Point (f_index row, f_index column);

        bool hasValue () const;

        int compare (const Point &that) const;

        void translate (const Point &delta);

        void translate (f_index down, f_index across);

        void traverse (const Point &delta);

        void traverse (f_index down, f_index across);

        string toString () const;

        string toExtendedString (const string &fileName) const;

        bool operator== (const Point &that) const;

        bool operator!= (const Point &that) const;

        bool operator< (const Point &that) const;

        bool operator<= (const Point &that) const;
    };

    struct Range {
        Point start;
        Point end;

        Range () = delete;

        explicit Range (Point point);

        Range (Point start, Point end);

        bool hasValue () const;

        string toString () const;

        Range merge (const Range &that) const;

        void translate (const Point &startDelta);

        void translate (const Point &startDelta, const Point &endDelta);

        void traverse (const Point &delta);

        int compare (const Range &that) const;

        bool containsPoint (const Point &point, bool exclusive = false) const;

        bool containsPoint (f_index row, f_index column, bool exclusive = false) const;

        bool containsRange (const Range &that, bool exclusive = false) const;

        bool intersectsWith (const Range &that, bool exclusive = false) const;

        bool coversSameRows (const Range &that);

        bool intersectsRow (int row) const;

        bool intersectsRowRange (int startRow, int endRow) const;

        bool operator== (const Range &that) const;

        bool operator!= (const Range &that) const;
    };

}
#endif //LANGUAGESERVER_TOOLS_TYPES_H
