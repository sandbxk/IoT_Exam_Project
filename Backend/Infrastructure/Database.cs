using Application.Interfaces;

namespace Infrastructure;

public class Database : IDatabase
{
    private DatabaseContext _context;
    
    public Database(DatabaseContext context)
    {
        _context = context;
    }

    public void build()
    {
        _context.Database.EnsureDeleted();
        _context.Database.EnsureCreated();
    }
}