using Domain;

namespace Application.Interfaces;

public interface IMqttRepository
{
    public Mqtt read(int id);
    public List<Mqtt> readAll();
    public Mqtt update(Mqtt mqtt);
    public Mqtt create(Mqtt mqtt);
    public int delete(int id);
}