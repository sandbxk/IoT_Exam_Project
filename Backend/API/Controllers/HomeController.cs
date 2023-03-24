using Application.Interfaces;
using Domain;
using Microsoft.AspNetCore.Mvc;

namespace API.Controllers;

[ApiController]
[Route("[controller]")]
public class HomeController : ControllerBase
{
    private readonly IMqttService _mqttService;
    public HomeController(IMqttService mqttService)
    {
        _mqttService = mqttService;
    }
    
    [Route("{id}")]
    [HttpGet]
    public ActionResult<Mqtt> Get([FromRoute] int id)
    {
        return _mqttService.read(id);
    }
    
    [HttpGet]
    public ActionResult<List<Mqtt>> GetAll()
    {
        return _mqttService.readAll();
    }
    
    [HttpPost]
    public ActionResult<Mqtt> Post([FromBody] Mqtt mqtt)
    {
        return _mqttService.create(mqtt);
    }
    
    [HttpDelete]
    public ActionResult<Boolean> Delete([FromBody] Mqtt mqtt)
    {
        return _mqttService.delete(mqtt);
    }
    
    [HttpPut]
    public ActionResult<Mqtt> Put([FromBody] Mqtt mqtt)
    {
        return _mqttService.update(mqtt);
    }
}