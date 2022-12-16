#include <kernel/gdt.h>

// The GDT table
struct gdt_entry gdt[3];

// The GDT pointer
struct gdt_ptr gp;

// The null GDT entry
struct gdt_entry gdt_null = {
  .limit_low = 0,
  .base_low = 0,
  .base_middle = 0,
  .access = 0,
  .granularity = 0,
  .base_high = 0
};

// The code segment GDT entry
struct gdt_entry gdt_code = {
  .limit_low = 0xFFFF,
  .base_low = 0,
  .base_middle = 0,
  .access = 0x9A,
  .granularity = 0xCF,
  .base_high = 0
};

// The data segment GDT entry
struct gdt_entry gdt_data = {
  .limit_low = 0xFFFF,
  .base_low = 0,
  .base_middle = 0,
  .access = 0x92,
  .granularity = 0xCF,
  .base_high = 0
};

// Initialize the GDT
void gdt_init(void)
{
  // Set the GDT pointer
  gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
  gp.base = (uint32_t)&gdt;

  // Set the null GDT entry
  gdt_set_entry(0, 0, 0, 0, 0);

  // Set the code segment GDT entry
  gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

  // Set the data segment GDT entry
  gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

}

// Set a GDT entry
void gdt_set_entry(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
  // Set the base address of the GDT entry
  gdt[num].base_low = (base & 0xFFFF);
  gdt[num].base_middle = (base >> 16) & 0xFF;
  gdt[num].base_high = (base >> 24) & 0xFF;

  // Set the limit of the GDT entry
  gdt[num].limit_low = (limit & 0xFFFF);
  gdt[num].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);

  // Set the access flags of the GDT entry
  gdt[num].access = access;

}
