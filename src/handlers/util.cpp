#include "util.h"

Point fromTSPoint (TSPoint tsPoint) {
    return Point { tsPoint.row, tsPoint.column >> 1 };
}

TSPoint toTSPoint (Point point) {
    return TSPoint { point.row, point.column << 1 };
}

Range fromTSRange (TSRange tsRange) {
    return { fromTSPoint(tsRange.start_point), fromTSPoint(tsRange.end_point) };
}

TSRange toTSRange (Range range) {
    return { toTSPoint(range.start), toTSPoint(range.end) };
}

TSRange ts_node_range (TSNode node) {
    return TSRange { ts_node_start_point(node), ts_node_end_point(node) };
}

Range nodeRange (TSNode node) {
    return fromTSRange(ts_node_range(node));
}
