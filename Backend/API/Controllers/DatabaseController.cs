using Application.Interfaces;
using Microsoft.AspNetCore.Mvc;

namespace API.Controllers;

[ApiController]
[Route("[controller]")]
public class DatabaseController: ControllerBase
{
    private readonly IDatabase _database;

    public DatabaseController(IDatabase database)
    {
        _database = database;
    }
    
    [HttpGet]
    [Route("buildDB")]
    public string CreateDatabase()
    {
        _database.build();
        return "Database created";
    }
}