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

    public Mqtt read(int id)
    {
        return _context.MqttTable.Find(id);
    }

    public List<Mqtt> readAll()
    {
        return _context.MqttTable.ToList();
    }

    public Mqtt update(Mqtt mqtt)
    {
        _context.MqttTable.Update(mqtt);
        _context.SaveChanges();
        return _context.MqttTable.Find(mqtt.Id);
    }

    public Mqtt create(Mqtt mqtt)
    {
        _context.MqttTable.Add(mqtt);
        _context.SaveChanges();
        return _context.MqttTable.Find(mqtt.Id);
    }

    public int delete(int id)
    {
        Mqtt mqtt = _context.MqttTable.Find(id);
        _context.MqttTable.Remove(mqtt);
        return _context.SaveChanges();
    }
}