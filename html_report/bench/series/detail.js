function makeLineFunction(serie, op) {
    return function(test) 
    {
        if (op == undefined) { 
            op = function(x) { return x; }
        }

        var lines = [];
        var samples = test[serie];
        var lineCount = samples[0].length - 1;
        for (var i=0; i<samples.length; i++)
        {
            var current = samples[i];

            for (var j=0;j<lineCount;j++)
            {
                if (lines[j] == undefined) lines[j] = [];

                var x, t, x0, t0;
                x = samples[i][j+1];
                t = samples[i][0];
                if (i > 0) { 
                    x0 = samples[i-1][j+1];
                    t0 = samples[i-1][0];
                } 

                lines[j].push({
                    "time": t,
                    "value": op(x, t, x0, t0, test)
                });
            }
        }
        return lines;
    };
}

function lineSeriesFromTest(test) {
    var series = defaultLineSeries;

    for (var key in test) {
        if (optionalLineSeries[key]) {
            series.push(optionalLineSeries[key]);
        } else {
            console.warn("unable to find series: ", key)
        }
    }

    return series;
}

var lineSeries = [];

var defaultLineSeries = [
    {
        "name": "Frequency",
        "unit": unit.kilohertz,
        "lines": makeLineFunction("iterations", function(x, t, x0, t0) {
            if (t0 == undefined) return 0;
            return (x-x0)/(t-t0);
        })
    },
    {
        "name": "Throughput",
        "unit": unit.kilobyte_per_second,
        "lines": makeLineFunction("iterations", function(x, t, x0, t0, test) {
            if (t0 == undefined) return 0;
            return (x-x0)*test.content_size/(t-t0);
        })
    }];

var optionalLineSeries = {
    "iterations": {
        "name": "Iterations",
        "unit": unit.none,
        "lines": makeLineFunction("iterations")
    },

    "successes": {
        "name": "Successes",
        "unit": unit.none,
        "lines": makeLineFunction("successes")
    },

    "failures": {
        "name": "Failures",
        "unit": unit.none,
        "lines": makeLineFunction("failures")
    },
    
    "vm_used": {
        "name": "Virtual memory usage", 
        "unit": unit.byte,
        "lines": makeLineFunction("vm_used")
    },

    "physmem_used": {
        "name": "Physical memory usage", 
        "unit": unit.byte,
        "lines": makeLineFunction("physmem_used")
    },

    "evictions": {
        "name": "Evictions",
        "unit": unit.none,
        "lines": makeLineFunction("evictions")
    },

    "persisted_size": {
        "name": "Persisted size",
        "unit": unit.byte,
        "lines": makeLineFunction("persisted_size")
    },

    "persisted_count": {
        "name": "Persisted count",
        "unit": unit.none,
        "lines": makeLineFunction("persisted_count")
    },

    "resident_size": {
        "name": "Resident size",
        "unit": unit.byte,
        "lines": makeLineFunction("resident_size")
    },

    "resident_count": {
        "name": "Resident count",
        "unit": unit.none,
        "lines": makeLineFunction("resident_count")
    },

    "db_objects": {
        "name": "Database object count",
        "unit": unit.none,
        "lines": makeLineFunction("db_objects")
    },

    "db_storage_size": {
        "name": "Database storage size",
        "unit": unit.byte,
        "lines": makeLineFunction("db_storage_size")
    },

    "server_objects": {
        "name": "Server object count",
        "unit": unit.none,
        "lines": makeLineFunction("server_objects")
    },

    "server_storage_size": {
        "name": "Server storage size",
        "unit": unit.byte,
        "lines": makeLineFunction("server_storage_size")
    }
};
