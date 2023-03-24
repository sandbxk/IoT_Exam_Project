using Application.Interfaces;

namespace Infrastructure;

public class MqttRepository : IMqttRepository
{
    DatabaseContext _context;
    
    MqttRepository(DatabaseContext context)
    {
        _context = context;
    }
}