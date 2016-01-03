// Example taken from http://bl.ocks.org/mbostock/3884955

jQuery( document ).ready(function() {

  var margin = {top: 20, right: 20, bottom: 30, left: 50},
      width = 620 - margin.left - margin.right,
      height = 330 - margin.top - margin.bottom;

  var parseDate = d3.time.format("%d-%b-%y").parse;

  var color = d3.scale.category10();

  var x = d3.time.scale()
      .range([0, width]);

  var y = d3.scale.linear()
      .range([height, 0]);

  var xAxis = d3.svg.axis()
      .scale(x)
      .orient("bottom");

  var yAxis = d3.svg.axis()
      .scale(y)
      .orient("left");

  var line = d3.svg.line()
      .x(function(d) { return x(d.date); })
      .y(function(d) { return y(d.score); });

  var svg = d3.select(".pp-comparing-chart").append("svg")
      .attr("width", width + margin.left + margin.right)
      .attr("height", height + margin.top + margin.bottom)
    .append("g")
      .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

  d3.tsv(Drupal.settings.pp_comparing.data, function(error, data) {
    color.domain(d3.keys(data[0]).filter(function (key) {return key !== 'date'} ));

    data.forEach(function(d) {
      d.date = parseDate(d.date);
    });

    var users = color.domain().map(function (name) {
      var user_score = 0;
      return {
        name: name,
        values: data.map(function (d) {
          user_score += parseInt(d[name]);
          return {
            date: d.date, score: user_score
          };
        })
      }
    });

    x.domain(d3.extent(data, function(d) { return d.date; }));
    y.domain([
      d3.min(users, function (c) { return d3.min(c.values, function (v) { return v.score; }); }),
      d3.max(users, function (c) {return d3.max(c.values, function (v) { return v.score; }); })
    ]);

    svg.append("g")
        .attr("class", "x axis")
        .attr("transform", "translate(0," + height + ")")
        .call(xAxis);

    svg.append("g")
        .attr("class", "y axis")
        .call(yAxis)
      .append("text")
        .attr("transform", "rotate(-90)")
        .attr("y", 6)
        .attr("dy", ".71em")
        .style("text-anchor", "end")
        .text("Number");

    var user = svg.selectAll('.city')
      .data(users)
      .enter().append("g")
      .attr("class", "user");

    user.append("path")
      .attr("class", "line")
      .attr("d", function(d) { return line(d.values); })
      .style("stroke", function(d) { return color(d.name); });

    user.append("text")
      .datum(function(d) { return {name: d.name, value: d.values[d.values.length - 1]}; })
      .attr("transform", function(d) { return "translate(" + x(d.value.date) + "," + y(d.value.score) + ")"; })
      .attr("x", 3)
      .attr("dy", ".35em")
      .text(function(d) { return d.name; });
  });

});
