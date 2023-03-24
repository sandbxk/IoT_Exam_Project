using Application.Interfaces;
using Domain;

namespace Infrastructure;

public class MqttRepository : IMqttRepository
{
    DatabaseContext _context;
    
    public MqttRepository(DatabaseContext context)
    {
        _context = context;
    }
}