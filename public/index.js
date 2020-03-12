// Put all onload AJAX calls here, and event listeners
$(document).ready(function() {
    // On page-load AJAX Example
    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything
        url: '/someendpoint',   //The server endpoint we are connecting to
        data: {
            name1: "Value 1",
            name2: "Value 2"
        },
        success: function (data) {
            /*  Do something with returned object
                Note that what we get is an object, not a string,
                so we do not need to parse it on the server.
                JavaScript really does handle JSONs seamlessly
            */
            $('#blah').html("On page load, received string '"+data.foo+"' from server");
            //We write the object to the console to show that the request was successful
            console.log(data);

        },
        fail: function(error) {
            // Non-200 return, do something with error
            $('#blah').html("On page load, received error from server");
            console.log(error);
        }
    });

    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/images',
        success: function (data) {
          for(i in data){
            var row = $("#file-log-table").append(
              "<tr>" +
                "<td><a download="+ i +" href=" + i + "><img src=" + i + " width=\"200px\"></a></td>" +
                "<td><a download="+ i +" href=" + i + ">" + i + "</a></td>" +
                "<td>" + data[i]["size"] + "</td>" +
                "<td>" + data[i]["numRect"] + "</td>" +
                "<td>" + data[i]["numCirc"] + "</td>" +
                "<td>" + data[i]["numPaths"] + "</td>" +
                "<td>" + data[i]["numGroups"] + "</td>" +
              "</tr>"
            );
            $('#images-list').append($('<option>', {
              value: i,
              text: i
            }));
          }
          console.log(data);
        },
        fail: function(error) {
            // Non-200 return, do something with error
            //$('#blah').html("On page load, received error from server");
            alert(error);
        }
    });

    $('#images-list').change(function(){
      $.ajax({
          type: 'get',
          dataType: 'json',
          url: '/viewpanel',
          data: {
              file: $('#images-list option:selected').text()
          },
          success: function (data) {
            $("#image-img").html("<img src=\"" + $('#images-list option:selected').text() + "\" width=\"800px\">");
            $("#image-title").html(data["title"]);
            $("#image-desc").html(data["description"]);
            $("#view-table-foot").html(" ");
            $("#images-list-edit").html(" ");
            for (var i = 0; i < data["rectangles"].length; i++) {
              $("#view-table-foot").append(
                "<tr>" +
                  "<td>Rectangle " + (i+1) + "</td>" +
                  "<td>Upper left corner: x = " + data["rectangles"][i]["x"]+data["rectangles"][i]["units"] + ", y = " +
                  data["rectangles"][i]["y"]+data["rectangles"][i]["units"] + " Width: " + data["rectangles"][i]["w"] +
                  data["rectangles"][i]["units"] + ", Height: " + data["rectangles"][i]["h"]+data["rectangles"][i]["units"] +
                  "<td>" + data["rectangles"][i]["numAttr"] + "</td>" +
                "</tr>"
              );
              $('#images-list-edit').append($('<option>', {
                value: "Rectangle " + (i+1),
                text: "Rectangle " + (i+1)
              }));
            }
            for (var i = 0; i < data["circles"].length; i++) {
              $("#view-table-foot").append(
                "<tr>" +
                  "<td>Circle " + (i+1) + "</td>" +
                  "<td>Centre: x = " + data["circles"][i]["cx"]+data["circles"][i]["units"] + ", y = " +
                  data["circles"][i]["cy"]+data["circles"][i]["units"] + ", radius = " + data["circles"][i]["r"] +
                  data["circles"][i]["units"] + "</td>" +
                  "<td>" + data["circles"][i]["numAttr"] + "</td>" +
                "</tr>"
              );
              $('#images-list-edit').append($('<option>', {
                value: "Circle " + (i+1),
                text: "Circle " + (i+1)
              }));
            }
            for (var i = 0; i < data["paths"].length; i++) {
              $("#view-table-foot").append(
                "<tr>" +
                  "<td>Path " + (i+1) + "</td>" +
                  "<td>path data = " + data["paths"][i]["d"] + "</td>" +
                  "<td>" + data["paths"][i]["numAttr"] + "</td>" +
                "</tr>"
              );
              $('#images-list-edit').append($('<option>', {
                value: "Path " + (i+1),
                text: "Path " + (i+1)
              }));
            }
            for (var i = 0; i < data["groups"].length; i++) {
              $("#view-table-foot").append(
                "<tr>" +
                  "<td>Group " + (i+1) + "</td>" +
                  "<td>" + data["groups"][i]["children"] + " child elements</td>" +
                  "<td>" + data["groups"][i]["numAttr"] + "</td>" +
                "</tr>"
              );
              $('#images-list-edit').append($('<option>', {
                value: "Group " + (i+1),
                text: "Group " + (i+1)
              }));
            }
            console.log(data);
          },
          fail: function(error) {
              //$('#blah').html("On page load, received error from server");
              alert(error);
          }
    });
  });

    // Event listener form example , we can use this instead explicitly listening for events
    // No redirects if possible
    $('#someform').submit(function(e){
        $('#blah').html("Form has data: "+$('#entryBox').val());
        e.preventDefault();
        //Pass data to the Ajax call, so it gets passed to the server
        $.ajax({
            //Create an object for connecting to another waypoint
        });
    });

    $('#edit-form').submit(function(e){
      e.preventDefault();
      let selected = $('#images-list-edit option:selected').text();
      if (selected == "Attributes" || $('#edit-name').val() == '' || $('#edit-name').val() == '') {
        alert("No attrs picked or feild is missing!");
        return;
      }
      var shape = selected.split(" ");

      console.log(selected);
      $.ajax({
          type: 'get',
          dataType: 'json',
          url: '/edit',
          data: {
              index: shape[1]-1,
              shape: shape[0],
              image: $('#images-list option:selected').text(),
              name: $('#edit-name').val(),
              value: $('#edit-value').val()
          },
          success: function (data) {
              console.log(data);
          },
          fail: function(error) {
              console.log(error);
          }
      });
    });
});
