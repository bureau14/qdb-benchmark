var renderTimeout;
var tests_by_class = {};
var test_by_id = [];

function addTestResult(test) {
    test.id = test_by_id.length;
    test_by_id.push(test);

    lineSeries = lineSeriesFromTest(test);

    if (tests_by_class[test.name] != undefined)
        tests_by_class[test.name].push(test);
    else 
        tests_by_class[test.name] = [test];

    clearTimeout(renderTimeout);
    renderTimeout = setTimeout(render);
}

var content = d3.select("#content");

function add_charts_for_test_class(test_name) {  

    var div = d3.select(this);
    var test = tests_by_class[test_name];

    div.attr("id", test_name);
    div.append("h2").text(test_name);
    div.append("p").text(test[0].description);

    var columnCount = bench.getContentSizes(test).length;
    var rowCount = bench.getThreadCounts(test).length;

    if (columnCount > 1 || rowCount > 1) {
        var summaryDiv = div.append("div").classed("summary", true);
        var summaryChart;

        if (columnCount == 1) {
            summaryChart = bench.chart.horizontal();
        } else if (rowCount == 1) {
            summaryChart = bench.chart.vertical();
        } else {
            summaryChart = bench.chart.bubble();
        } 

        summaryChart.data(test);     
        summaryChart(summaryDiv);
        summaryChart.on("select", function(id){
            console.log("test "+id);   
            detailChart.data(test_by_id[id]);  
            detailChart.update();      
        });
    }
    
    var detailDiv = div.append("div").classed("detail", true);
    var detailChart = bench.chart.lineChart();
    detailChart.data(test[0]);
    detailChart(detailDiv);
}

function render()
{
    var class_names = Object.keys(tests_by_class);

    content
        .append("h1")
            .attr("id", "overview")
            .text("Benchmark Results");

    var overview = bench.chart.overview();
    overview.data(d3.values(tests_by_class));
    overview.on("checked", function(test, checked) {
        d3.select("#"+test).style("display", checked ? "block" : "none");
    });
    overview(content.append("div").classed("overview", true));

    content
        .selectAll("div.test")
        .data(class_names)
        .enter()
        .append("div").classed("test", true)
        .each(add_charts_for_test_class);
}
