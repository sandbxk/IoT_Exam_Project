using Domain;
using Microsoft.EntityFrameworkCore;

namespace Infrastructure;

public class DatabaseContext : DbContext
{
    public DatabaseContext(DbContextOptions<DatabaseContext> options) : base(options) {}

    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        //Setting Primary Keys
        modelBuilder.Entity<Mqtt>()
            .HasKey(m => m.Id);
        
        //Auto ID generation
        modelBuilder.Entity<Mqtt>()
            .Property(m => m.Id)
            .ValueGeneratedOnAdd();
    }
    public DbSet<Mqtt> MqttTable { get; set; }
}