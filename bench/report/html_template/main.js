var test_per_class = {};
results.forEach(function(x, i) {
    x.id = i;
    if (test_per_class[x.name] != undefined)
        test_per_class[x.name].push(x);
    else 
        test_per_class[x.name] = [x];
});
var class_names = Object.keys(test_per_class);

var content = d3.select("#content");

function add_charts_for_test_class(test)
{
    var div = content.append("div").classed("test", true);

    div.append("h1").text(test[0].name);
    div.append("p").text(test[0].description);

    var gridChart = d3.chart.speedGridChart();
    gridChart.data(test);        

    gridChart(div);

    var detailChart = d3.chart.threadSpeedChart();
    detailChart.data(results[0]);
    detailChart(div);

    gridChart.on("select", function(id){
        console.log("test "+id);   
        detailChart.data(results[id]);  
        detailChart.update();      
    });
}

for (var key in test_per_class)
{
    if (test_per_class[key][0].content_size>0)
    {
        add_charts_for_test_class(test_per_class[key]);
    }
}