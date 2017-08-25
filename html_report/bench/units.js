if (typeof bench == "undefined") bench = {};

bench.units = (function(){

    function formatValue(value, div, unit) {
        value = parseFloat(value);
        var mulipliers = ["", "k", "M", "G", "T", "P"];
        var m = 0;
        while ((value>=div || value<=-div) && m<mulipliers.length-1) {
            value/=div;
            m++;
        }
        var suffix = " " + mulipliers[m] + unit;
        if (value < 10) return value.toFixed(1) + suffix;
        else return value.toFixed(0) + suffix;
    }

    function formatTime(value, index) {
        var multipliers = ["ns", "\xB5s", "ms", "s", "min"];
        var divisors = [1000,1000,1000,60,60]
        value = parseFloat(value);
        var m = index;
        while (value >= divisors[m] && m < divisors.length) {
            value /= divisors[m];
            m++;
        }
        return value + " " + multipliers[m]
    }

    return {
        "none": function(x) {
            return formatValue(x, 1000, "")
        },

        "bytes": function(x) {
            return formatValue(x, 1024, "B")
        },

        "bytes/second": function(x) {
            return formatValue(x, 1024, "B/s");
        },

        "kilobytes/second": function(x) {
            return formatValue(x*1000, 1024, "B/s");
        },

        "hertz": function(x) {
            return formatValue(x, 1000, "Hz");
        },

        "kilohertz": function(x) {
            return formatValue(x*1000, 1000, "Hz");
        },

        "millisecond": function(x) {
            return formatValue(x/1000.0, 1000, "s")
        },

        "nanosecond": function(x) {
            return formatTime(x, 0)
        }
    }
})();
